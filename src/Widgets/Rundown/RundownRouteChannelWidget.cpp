#include "RundownRouteChannelWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Events/ConnectionStateChangedEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <QtWidgets/QGraphicsOpacityEffect>

RundownRouteChannelWidget::RundownRouteChannelWidget(const LibraryModel& model, QWidget* parent, const QString& color,
                                                     bool active, bool loaded, bool paused, bool playing, bool inGroup,
                                                     bool compactView)
    : QWidget(parent),
      active(active), loaded(loaded), paused(paused), playing(playing), inGroup(inGroup), compactView(compactView), color(color),
      model(model), stopControlSubscription(NULL), playControlSubscription(NULL), playNowControlSubscription(NULL), loadControlSubscription(NULL),
      pauseControlSubscription(NULL), clearControlSubscription(NULL), clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();
    this->markUsedItems = (DatabaseManager::getInstance().getConfigurationByName("MarkUsedItems").getValue() == "true") ? true : false;

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_PRODUCER_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.channel.get()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.videolayer.get()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command.channel, SIGNAL(changed(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command.videolayer, SIGNAL(changed(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(remoteTriggerIdChanged(const QString&)), this, SLOT(remoteTriggerIdChanged(const QString&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(labelChanged(const LabelChangedEvent&)), this, SLOT(labelChanged(const LabelChangedEvent&)));

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();
}

void RundownRouteChannelWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->selected)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel());
}

void RundownRouteChannelWidget::deviceChanged(const DeviceChangedEvent& event)
{
    // This event is not for us.
    if (!this->selected)
        return;

    // Should we update the device name?
    if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model.getDeviceName())
    {
        // Disconnect connectionStateChanged() from the old device.
        const QSharedPointer<CasparDevice> oldDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (oldDevice != NULL)
            QObject::disconnect(oldDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

        // Update the model with the new device.
        this->model.setDeviceName(event.getDeviceName());
        this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

        // Connect connectionStateChanged() to the new device.
        const QSharedPointer<CasparDevice> newDevice = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (newDevice != NULL)
            QObject::connect(newDevice.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
    }

    checkEmptyDevice();
    checkDeviceConnection();
}

AbstractRundownWidget* RundownRouteChannelWidget::clone()
{
    RundownRouteChannelWidget* widget = new RundownRouteChannelWidget(this->model, this->parentWidget(), this->color,
                                                                      this->active, this->loaded, this->paused, this->playing,
                                                                      this->inGroup, this->compactView);

    RouteChannelCommand* command = dynamic_cast<RouteChannelCommand*>(widget->getCommand());
    command->channel.set(this->command.channel.get());
    command->videolayer.set(this->command.videolayer.get());
    command->setDelay(this->command.getDelay());
    command->setDuration(this->command.getDuration());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setRemoteTriggerId(this->command.getRemoteTriggerId());
    command->setFromChannel(this->command.getFromChannel());

    return widget;
}

void RundownRouteChannelWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
    }

    this->compactView = compactView;
}

void RundownRouteChannelWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownRouteChannelWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownRouteChannelWidget::isGroup() const
{
    return false;
}

bool RundownRouteChannelWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownRouteChannelWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownRouteChannelWidget::getLibraryModel()
{
    return &this->model;
}

void RundownRouteChannelWidget::setSelected(bool selected)
{
    this->selected = selected;
}

void RundownRouteChannelWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownRouteChannelWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

QString RundownRouteChannelWidget::getColor() const
{
    return this->color;
}

void RundownRouteChannelWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownRouteChannelWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: firebrick;");
    else
        this->labelDevice->setStyleSheet("");
}

void RundownRouteChannelWidget::clearDelayedCommands()
{
    this->executeTimer.stop();
}

void RundownRouteChannelWidget::setUsed(bool used)
{
    if (used)
    {
        if (this->frameItem->graphicsEffect() == NULL)
        {
            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.25);

            this->frameItem->setGraphicsEffect(effect);
        }
    }
    else
        this->frameItem->setGraphicsEffect(NULL);
}

bool RundownRouteChannelWidget::executeCommand(Playout::PlayoutType type)
{
    if (type == Playout::PlayoutType::Stop)
        executeStop();
    else if (type == Playout::PlayoutType::Play)
    {
        if (this->command.getDelay() < 0)
            return true;

        if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
        {
            if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
            {
                const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
                if (this->command.channel.get() > channelFormats.count())
                    return true;

                double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.channel.get() - 1]).getFramesPerSecond().toDouble();

                int startDelay = floor(this->command.getDelay() * (1000 / framesPerSecond));
                this->executeTimer.setInterval(startDelay);

                if (this->command.getDuration() > 0)
                {
                    int stopDelay = floor(this->command.getDuration() * (1000 / framesPerSecond));
                    QTimer::singleShot(startDelay + stopDelay, this, SLOT(executeStop()));
                }
            }
            else if (this->delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
            {
                this->executeTimer.setInterval(this->command.getDelay());

                if (this->command.getDuration() > 0)
                    QTimer::singleShot(this->command.getDelay() + this->command.getDuration(), this, SLOT(executeStop()));
            }

            this->executeTimer.start();
        }
    }
    else if (type == Playout::PlayoutType::PlayNow)
        executePlay();
    else if (type == Playout::PlayoutType::PauseResume)
        executePause();
    else if (type == Playout::PlayoutType::Load)
        executeLoad();
    else if (type == Playout::PlayoutType::Clear)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearVideoLayer)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearChannel)
        executeClearChannel();

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownRouteChannelWidget::executeStop()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->stop(this->command.channel.get(), this->command.videolayer.get());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->stop(this->command.channel.get(), this->command.videolayer.get());
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownRouteChannelWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
            device->play(this->command.channel.get(), this->command.videolayer.get());
        else
            device->playRouteChannel(this->command.channel.get(), this->command.videolayer.get(), this->command.getFromChannel());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->loaded)
                deviceShadow->play(this->command.channel.get(), this->command.videolayer.get());
            else
                deviceShadow->playRouteChannel(this->command.channel.get(), this->command.videolayer.get(), this->command.getFromChannel());
        }
    }

    if (this->markUsedItems)
        setUsed(true);

    this->paused = false;
    this->loaded = false;
    this->playing = true;
}

void RundownRouteChannelWidget::executePause()
{
    if (!this->playing)
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->paused)
            device->resume(this->command.channel.get(), this->command.videolayer.get());
        else
            device->pause(this->command.channel.get(), this->command.videolayer.get());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->paused)
                deviceShadow->resume(this->command.channel.get(), this->command.videolayer.get());
            else
                deviceShadow->pause(this->command.channel.get(), this->command.videolayer.get());
        }
    }

    this->paused = !this->paused;
}

void RundownRouteChannelWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->loadRouteChannel(this->command.channel.get(), this->command.videolayer.get(), this->command.getFromChannel());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->loadRouteChannel(this->command.channel.get(), this->command.videolayer.get(), this->command.getFromChannel());
    }

    this->loaded = true;
    this->paused = false;
    this->playing = false;
}

void RundownRouteChannelWidget::executeClearVideolayer()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearVideolayer(this->command.channel.get(), this->command.videolayer.get());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearVideolayer(this->command.channel.get(), this->command.videolayer.get());
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownRouteChannelWidget::executeClearChannel()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.channel.get());
        device->clearMixerChannel(this->command.channel.get());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearChannel(this->command.channel.get());
            deviceShadow->clearMixerChannel(this->command.channel.get());
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownRouteChannelWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownRouteChannelWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownRouteChannelWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownRouteChannelWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownRouteChannelWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownRouteChannelWidget::configureOscSubscriptions()
{
    if (!this->command.getAllowRemoteTriggering())
        return;

    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowControlSubscription != NULL)
        this->playNowControlSubscription->disconnect(); // Disconnect all events.

    if (this->loadControlSubscription != NULL)
        this->loadControlSubscription->disconnect(); // Disconnect all events.

    if (this->pauseControlSubscription != NULL)
        this->pauseControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.

    QString stopControlFilter = Osc::ITEM_CONTROL_STOP_FILTER;
    stopControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->stopControlSubscription = new OscSubscription(stopControlFilter, this);
    QObject::connect(this->stopControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(stopControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playControlFilter = Osc::ITEM_CONTROL_PLAY_FILTER;
    playControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playNowControlFilter = Osc::ITEM_CONTROL_PLAYNOW_FILTER;
    playNowControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playNowControlSubscription = new OscSubscription(playNowControlFilter, this);
    QObject::connect(this->playNowControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::ITEM_CONTROL_LOAD_FILTER;
    loadControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pauseControlFilter = Osc::ITEM_CONTROL_PAUSE_FILTER;
    pauseControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->pauseControlSubscription = new OscSubscription(pauseControlFilter, this);
    QObject::connect(this->pauseControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearControlFilter = Osc::ITEM_CONTROL_CLEAR_FILTER;
    clearControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearControlSubscription = new OscSubscription(clearControlFilter, this);
    QObject::connect(this->clearControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearVideolayerControlFilter = Osc::ITEM_CONTROL_CLEARVIDEOLAYER_FILTER;
    clearVideolayerControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearVideolayerControlSubscription = new OscSubscription(clearVideolayerControlFilter, this);
    QObject::connect(this->clearVideolayerControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearChannelControlFilter = Osc::ITEM_CONTROL_CLEARCHANNEL_FILTER;
    clearChannelControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearChannelControlSubscription = new OscSubscription(clearChannelControlFilter, this);
    QObject::connect(this->clearChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownRouteChannelWidget::allowGpiChanged(bool allowGpi)
{
    Q_UNUSED(allowGpi);

    checkGpiConnection();
}

void RundownRouteChannelWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    Q_UNUSED(connected);
    Q_UNUSED(device);

    checkGpiConnection();
}

void RundownRouteChannelWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownRouteChannelWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    Q_UNUSED(device);

    checkDeviceConnection();
}

void RundownRouteChannelWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress())->getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}

void RundownRouteChannelWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Stop);
}

void RundownRouteChannelWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownRouteChannelWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::PlayNow);
}

void RundownRouteChannelWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Load);
}

void RundownRouteChannelWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::PauseResume);
}

void RundownRouteChannelWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Clear);
}

void RundownRouteChannelWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearVideoLayer);
}

void RundownRouteChannelWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearChannel);
}
