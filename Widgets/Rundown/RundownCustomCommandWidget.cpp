#include "RundownCustomCommandWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "GpiManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/LabelChangedEvent.h"
#include "Events/DeviceChangedEvent.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownCustomCommandWidget::RundownCustomCommandWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                                       bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_MIXER_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));

    connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    qApp->installEventFilter(this);
}

bool RundownCustomCommandWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LabelChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        LabelChangedEvent* labelChanged = dynamic_cast<LabelChangedEvent*>(event);
        this->model.setLabel(labelChanged->getLabel());

        this->labelLabel->setText(this->model.getLabel());
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::DeviceChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        // Should we update the device name?
        DeviceChangedEvent* deviceChangedEvent = dynamic_cast<DeviceChangedEvent*>(event);
        if (deviceChangedEvent->getDeviceName() != this->model.getDeviceName())
        {
            // Disconnect connectionStateChanged() from the old device.
            const QSharedPointer<CasparDevice> oldDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (oldDevice != NULL)
                disconnect(oldDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

            // Update the model with the new device.
            this->model.setDeviceName(deviceChangedEvent->getDeviceName());
            this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

            // Connect connectionStateChanged() to the new device.
            const QSharedPointer<CasparDevice> newDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (newDevice != NULL)
                connect(newDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
        }

        checkEmptyDevice();
        checkDeviceConnection();
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownCustomCommandWidget::clone()
{
    RundownCustomCommandWidget* widget = new RundownCustomCommandWidget(this->model, this->parentWidget(), this->color, this->active,
                                                                        this->inGroup, this->compactView);

    CustomCommand* command = dynamic_cast<CustomCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setStopCommand(this->command.getStopCommand());
    command->setPlayCommand(this->command.getPlayCommand());
    command->setLoadCommand(this->command.getLoadCommand());
    command->setPauseCommand(this->command.getPauseCommand());
    command->setNextCommand(this->command.getNextCommand());
    command->setUpdateCommand(this->command.getUpdateCommand());
    command->setInvokeCommand(this->command.getInvokeCommand());
    command->setClearCommand(this->command.getClearCommand());
    command->setClearVideolayerCommand(this->command.getClearVideolayerCommand());
    command->setClearChannelCommand(this->command.getClearChannelCommand());

    return widget;
}

void RundownCustomCommandWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Define::COMPACT_ICON_WIDTH, Define::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::COMPACT_ICON_WIDTH, Define::COMPACT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Define::COMPACT_ICON_WIDTH, Define::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Define::DEFAULT_ICON_WIDTH, Define::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Define::DEFAULT_ICON_WIDTH, Define::DEFAULT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Define::DEFAULT_ICON_WIDTH, Define::DEFAULT_ICON_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownCustomCommandWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownCustomCommandWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownCustomCommandWidget::isGroup() const
{
    return false;
}

AbstractCommand* RundownCustomCommandWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownCustomCommandWidget::getLibraryModel()
{
    return &this->model;
}

void RundownCustomCommandWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownCustomCommandWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);

    if (this->inGroup)
    {
        this->labelDelay->setGeometry(this->labelDelay->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                      this->labelDelay->geometry().y(),
                                      this->labelDelay->geometry().width(),
                                      this->labelDelay->geometry().height());
        this->labelDevice->setGeometry(this->labelDevice->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                       this->labelDevice->geometry().y(),
                                       this->labelDevice->geometry().width(),
                                       this->labelDevice->geometry().height());
    }
}

void RundownCustomCommandWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownCustomCommandWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownCustomCommandWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::Play)
    {
        this->executeTimer.setInterval(this->command.getDelay());
        this->executeTimer.start();
    }
    else if (type == Playout::PlayoutType::Load)
        QTimer::singleShot(0, this, SLOT(executeLoad()));
    else if (type == Playout::PlayoutType::Pause)
        QTimer::singleShot(0, this, SLOT(executePause()));
    else if (type == Playout::PlayoutType::Next)
        QTimer::singleShot(0, this, SLOT(executeNext()));
    else if (type == Playout::PlayoutType::Update)
        QTimer::singleShot(0, this, SLOT(executeUpdate()));
    else if (type == Playout::PlayoutType::Invoke)
        QTimer::singleShot(0, this, SLOT(executeInvoke()));
    else if (type == Playout::PlayoutType::Clear)
        QTimer::singleShot(0, this, SLOT(executeClear()));
    else if (type == Playout::PlayoutType::ClearVideolayer)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearChannel)
        QTimer::singleShot(0, this, SLOT(executeClearChannel()));

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownCustomCommandWidget::executeStop()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getStopCommand().isEmpty())
        device->sendCommand(this->command.getStopCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getStopCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getStopCommand());
    }
}

void RundownCustomCommandWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getPlayCommand().isEmpty())
        device->sendCommand(this->command.getPlayCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getPlayCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getPlayCommand());
    }
}

void RundownCustomCommandWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getLoadCommand().isEmpty())
        device->sendCommand(this->command.getLoadCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getLoadCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getLoadCommand());
    }
}

void RundownCustomCommandWidget::executePause()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getPauseCommand().isEmpty())
        device->sendCommand(this->command.getPauseCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getPauseCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getPauseCommand());
    }
}

void RundownCustomCommandWidget::executeNext()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getNextCommand().isEmpty())
        device->sendCommand(this->command.getNextCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getNextCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getNextCommand());
    }
}

void RundownCustomCommandWidget::executeUpdate()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getUpdateCommand().isEmpty())
        device->sendCommand(this->command.getUpdateCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getUpdateCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getUpdateCommand());
    }
}

void RundownCustomCommandWidget::executeInvoke()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getInvokeCommand().isEmpty())
        device->sendCommand(this->command.getInvokeCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getInvokeCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getInvokeCommand());
    }
}

void RundownCustomCommandWidget::executeClear()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getClearCommand().isEmpty())
        device->sendCommand(this->command.getClearCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getClearCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getClearCommand());
    }
}

void RundownCustomCommandWidget::executeClearVideolayer()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getClearVideolayerCommand().isEmpty())
        device->sendCommand(this->command.getClearVideolayerCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getClearVideolayerCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getClearVideolayerCommand());
    }
}

void RundownCustomCommandWidget::executeClearChannel()
{
    this->executeTimer.stop();

    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected() && !this->command.getClearChannelCommand().isEmpty())
        device->sendCommand(this->command.getClearChannelCommand());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected() && !this->command.getClearChannelCommand().isEmpty())
            deviceShadow->sendCommand(this->command.getClearChannelCommand());
    }
}

void RundownCustomCommandWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownCustomCommandWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownCustomCommandWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownCustomCommandWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownCustomCommandWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownCustomCommandWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownCustomCommandWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}
