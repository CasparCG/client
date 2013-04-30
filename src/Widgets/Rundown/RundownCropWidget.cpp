#include "RundownCropWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "GpiManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/LabelChangedEvent.h"
#include "Events/DeviceChangedEvent.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownCropWidget::RundownCropWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
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
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    qApp->installEventFilter(this);
}

bool RundownCropWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::Preview))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        executePlay();

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LabelChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        LabelChangedEvent* labelChanged = dynamic_cast<LabelChangedEvent*>(event);
        this->model.setLabel(labelChanged->getLabel());

        this->labelLabel->setText(this->model.getLabel());

        return true;
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::DeviceChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        // Should we update the device name?
        DeviceChangedEvent* deviceChangedEvent = dynamic_cast<DeviceChangedEvent*>(event);
        if (!deviceChangedEvent->getDeviceName().isEmpty() && deviceChangedEvent->getDeviceName() != this->model.getDeviceName())
        {
            // Disconnect connectionStateChanged() from the old device.
            const QSharedPointer<CasparDevice> oldDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (oldDevice != NULL)
                QObject::disconnect(oldDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

            // Update the model with the new device.
            this->model.setDeviceName(deviceChangedEvent->getDeviceName());
            this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

            // Connect connectionStateChanged() to the new device.
            const QSharedPointer<CasparDevice> newDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
            if (newDevice != NULL)
                QObject::connect(newDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
        }

        checkEmptyDevice();
        checkDeviceConnection();
    }

    return QObject::eventFilter(target, event);
}

AbstractRundownWidget* RundownCropWidget::clone()
{
    RundownCropWidget* widget = new RundownCropWidget(this->model, this->parentWidget(), this->color, this->active,
                                                      this->inGroup, this->compactView);

    CropCommand* command = dynamic_cast<CropCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setCropLeft(this->command.getCropLeft());
    command->setCropRight(this->command.getCropRight());
    command->setCropTop(this->command.getCropTop());
    command->setCropBottom(this->command.getCropBottom());
    command->setDuration(this->command.getDuration());
    command->setTween(this->command.getTween());
    command->setDefer(this->command.getDefer());

    return widget;
}

void RundownCropWidget::setCompactView(bool compactView)
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

void RundownCropWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownCropWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownCropWidget::isGroup() const
{
    return false;
}

AbstractCommand* RundownCropWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownCropWidget::getLibraryModel()
{
    return &this->model;
}

void RundownCropWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownCropWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);

    if (this->inGroup)
    {
        this->labelChannel->setGeometry(this->labelChannel->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                        this->labelChannel->geometry().y(),
                                        this->labelChannel->geometry().width(),
                                        this->labelChannel->geometry().height());
        this->labelVideolayer->setGeometry(this->labelVideolayer->geometry().x() + Define::GROUP_XPOS_OFFSET,
                                           this->labelVideolayer->geometry().y(),
                                           this->labelVideolayer->geometry().width(),
                                           this->labelVideolayer->geometry().height());
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

void RundownCropWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: rgba(%1); }").arg(color));
}

void RundownCropWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownCropWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::Play || type == Playout::PlayoutType::Update)
    {
        this->executeTimer.setInterval(this->command.getDelay());
        this->executeTimer.start();
    }
    else if (type == Playout::PlayoutType::Clear)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::ClearVideolayer)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearChannel)
        QTimer::singleShot(0, this, SLOT(executeClearChannel()));

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownCropWidget::executeStop()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setClipping(this->command.getChannel(), this->command.getVideolayer(), 0, 0, 1, 1);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setClipping(this->command.getChannel(), this->command.getVideolayer(), 0, 0, 1, 1);
    }
}

void RundownCropWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setClipping(this->command.getChannel(), this->command.getVideolayer(), this->command.getCropLeft(),
                            this->command.getCropRight(), this->command.getCropTop(), this->command.getCropBottom(),
                            this->command.getDuration(), this->command.getTween(), this->command.getDefer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setClipping(this->command.getChannel(), this->command.getVideolayer(), this->command.getCropLeft(),
                                      this->command.getCropRight(), this->command.getCropTop(), this->command.getCropBottom(),
                                      this->command.getDuration(), this->command.getTween(), this->command.getDefer());
    }
}

void RundownCropWidget::executeClearVideolayer()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearMixerVideolayer(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearMixerVideolayer(this->command.getChannel(), this->command.getVideolayer());
    }
}

void RundownCropWidget::executeClearChannel()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.getChannel());
        device->clearMixerChannel(this->command.getChannel());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearChannel(this->command.getChannel());
            deviceShadow->clearMixerChannel(this->command.getChannel());
        }
    }
}

void RundownCropWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownCropWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownCropWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownCropWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownCropWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownCropWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownCropWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownCropWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownCropWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}
