#include "RundownLevelsWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "GpiManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/RundownItemChangedEvent.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownLevelsWidget::RundownLevelsWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                         bool inGroup, bool disconnected)
    : QWidget(parent),
      active(active), inGroup(inGroup), disconnected(disconnected), color(color), model(model)
{
    setupUi(this);

    setActive(active);

    this->labelDisconnected->setVisible(this->disconnected);
    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(color));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)),
                     this, SLOT(gpiDeviceConnected(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiTriggerable();

    qApp->installEventFilter(this);
}

bool RundownLevelsWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        RundownItemChangedEvent* rundownItemChangedEvent = dynamic_cast<RundownItemChangedEvent*>(event);
        this->model.setLabel(rundownItemChangedEvent->getLabel());
        this->model.setDeviceName(rundownItemChangedEvent->getDeviceName());
        this->model.setName(rundownItemChangedEvent->getName());

        this->labelLabel->setText(this->model.getLabel());
        this->labelDevice->setText(QString("Device: %1").arg(this->model.getDeviceName()));

        checkEmptyDevice();
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemPreview))
    {
        // This event is not for us.
        if (!this->active)
            return false;

        executePlay();
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::ConnectionStateChanged))
    {
        ConnectionStateChangedEvent* connectionStateChangedEvent = dynamic_cast<ConnectionStateChangedEvent*>(event);
        if (connectionStateChangedEvent->getDeviceName() == this->model.getDeviceName())
        {
            this->disconnected = !connectionStateChangedEvent->getConnected();

            if (connectionStateChangedEvent->getConnected())
                this->labelDisconnected->setVisible(false);
            else
                this->labelDisconnected->setVisible(true);
        }
    }

    return QObject::eventFilter(target, event);
}

IRundownWidget* RundownLevelsWidget::clone()
{
    RundownLevelsWidget* widget = new RundownLevelsWidget(this->model, this->parentWidget(), this->color, this->active,
                                                          this->inGroup, this->disconnected);

    LevelsCommand* command = dynamic_cast<LevelsCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setMinIn(this->command.getMinIn());
    command->setMaxIn(this->command.getMaxIn());
    command->setMinOut(this->command.getMinOut());
    command->setMaxOut(this->command.getMaxOut());
    command->setGamma(this->command.getGamma());
    command->setDuration(this->command.getDuration());
    command->setTween(this->command.getTween());
    command->setDefer(this->command.getDefer());

    return widget;
}

void RundownLevelsWidget::readProperties(boost::property_tree::wptree& pt)
{
    setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownLevelsWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownLevelsWidget::isGroup() const
{
    return false;
}

ICommand* RundownLevelsWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownLevelsWidget::getLibraryModel()
{
    return &this->model;
}

void RundownLevelsWidget::setActive(bool active)
{
    this->active = active;

    if (this->active)
        this->labelActiveColor->setStyleSheet("background-color: red;");
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownLevelsWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(inGroup);

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

void RundownLevelsWidget::setColor(const QString& color)
{
    this->color = color;
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(color));
}

void RundownLevelsWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: black;");
    else
        this->labelDevice->setStyleSheet("");
}

bool RundownLevelsWidget::executeCommand(enum Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        QTimer::singleShot(0, this, SLOT(executeStop()));
    else if (type == Playout::PlayoutType::Play ||
             type == Playout::PlayoutType::Invoke ||
             type == Playout::PlayoutType::Update)
        QTimer::singleShot(this->command.getDelay(), this, SLOT(executePlay()));
    else if (type == Playout::PlayoutType::Clear)
        QTimer::singleShot(0, this, SLOT(executeClear()));
    else if (type == Playout::PlayoutType::ClearVideolayer)
        QTimer::singleShot(0, this, SLOT(executeClearVideolayer()));
    else if (type == Playout::PlayoutType::ClearChannel)
        QTimer::singleShot(0, this, SLOT(executeClearChannel()));

    return true;
}

void RundownLevelsWidget::executeStop()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setLevels(this->command.getChannel(), this->command.getVideolayer(), 0, 1, 1, 0, 1);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setLevels(this->command.getChannel(), this->command.getVideolayer(), 0, 1, 1, 0, 1);
    }
}

void RundownLevelsWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setLevels(this->command.getChannel(), this->command.getVideolayer(), this->command.getMinIn(), this->command.getMaxIn(),
                          this->command.getGamma(), this->command.getMinOut(), this->command.getMaxOut(), this->command.getDuration(),
                          this->command.getTween(), this->command.getDefer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setLevels(this->command.getChannel(), this->command.getVideolayer(), this->command.getMinIn(), this->command.getMaxIn(),
                                    this->command.getGamma(), this->command.getMinOut(), this->command.getMaxOut(), this->command.getDuration(),
                                    this->command.getTween(), this->command.getDefer());
    }
}

void RundownLevelsWidget::executeClear()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setLevels(this->command.getChannel(), this->command.getVideolayer(), 0, 1, 1, 0, 1);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setLevels(this->command.getChannel(), this->command.getVideolayer(), 0, 1, 1, 0, 1);
    }
}

void RundownLevelsWidget::executeClearVideolayer()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearMixerVideolayer(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearMixerVideolayer(this->command.getChannel(), this->command.getVideolayer());
    }
}

void RundownLevelsWidget::executeClearChannel()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.getChannel());
        device->clearMixerChannel(this->command.getChannel());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearChannel(this->command.getChannel());
            deviceShadow->clearMixerChannel(this->command.getChannel());
        }
    }
}

void RundownLevelsWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownLevelsWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownLevelsWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownLevelsWidget::checkGpiTriggerable()
{
    labelGpiTriggerable->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        labelGpiTriggerable->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        labelGpiTriggerable->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownLevelsWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiTriggerable();
}

void RundownLevelsWidget::gpiDeviceConnected(bool connected, GpiDevice* device)
{
    checkGpiTriggerable();
}
