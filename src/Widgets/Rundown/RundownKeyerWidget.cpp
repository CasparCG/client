#include "RundownKeyerWidget.h"

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

RundownKeyerWidget::RundownKeyerWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                       bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), inGroup(inGroup), compactView(compactView), color(color), model(model), stopControlSubscription(NULL),
      playControlSubscription(NULL), playNowControlSubscription(NULL), updateControlSubscription(NULL), clearControlSubscription(NULL),
      clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL)
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
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_MIXER_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.channel.get()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.videolayer.get()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.delay.get()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);
    QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

    QObject::connect(&this->command.channel, SIGNAL(changed(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command.videolayer, SIGNAL(changed(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command.delay, SIGNAL(changed(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command.allowGpi, SIGNAL(changed(bool)), this, SLOT(allowGpiChanged(bool)));
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

void RundownKeyerWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->selected)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel());
}

void RundownKeyerWidget::deviceChanged(const DeviceChangedEvent& event)
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

AbstractRundownWidget* RundownKeyerWidget::clone()
{
    RundownKeyerWidget* widget = new RundownKeyerWidget(this->model, this->parentWidget(), this->color, this->active,
                                                        this->inGroup, this->compactView);

    KeyerCommand* command = dynamic_cast<KeyerCommand*>(widget->getCommand());
    command->channel.set(this->command.channel.get());
    command->videolayer.set(this->command.videolayer.get());
    command->delay.set(this->command.delay.get());
    command->duration.set(this->command.duration.get());
    command->allowGpi.set(this->command.allowGpi.get());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setRemoteTriggerId(this->command.getRemoteTriggerId());

    return widget;
}

void RundownKeyerWidget::setCompactView(bool compactView)
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

void RundownKeyerWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownKeyerWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownKeyerWidget::isGroup() const
{
    return false;
}

bool RundownKeyerWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownKeyerWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownKeyerWidget::getLibraryModel()
{
    return &this->model;
}

void RundownKeyerWidget::setSelected(bool selected)
{
    this->selected = selected;
}

void RundownKeyerWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownKeyerWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

QString RundownKeyerWidget::getColor() const
{
    return this->color;
}

void RundownKeyerWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownKeyerWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: firebrick;");
    else
        this->labelDevice->setStyleSheet("");
}

void RundownKeyerWidget::clearDelayedCommands()
{
    this->executeTimer.stop();
}

void RundownKeyerWidget::setUsed(bool used)
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

bool RundownKeyerWidget::executeCommand(Playout::PlayoutType type)
{
    if (type == Playout::PlayoutType::Stop)
        executeStop();
    else if (type == Playout::PlayoutType::Play || type == Playout::PlayoutType::Update || type == Playout::PlayoutType::Load)
    {
        if (this->command.delay.get() < 0)
            return true;

        if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
        {
            if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
            {
                const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
                if (this->command.channel.get() > channelFormats.count())
                    return true;

                double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.channel.get() - 1]).getFramesPerSecond().toDouble();

                int startDelay = floor(this->command.delay.get() * (1000 / framesPerSecond));
                this->executeTimer.setInterval(startDelay);

                if (this->command.duration.get() > 0)
                {
                    int stopDelay = floor(this->command.duration.get() * (1000 / framesPerSecond));
                    QTimer::singleShot(startDelay + stopDelay, this, SLOT(executeStop()));
                }
            }
            else if (this->delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
            {
                this->executeTimer.setInterval(this->command.delay.get());

                if (this->command.duration.get() > 0)
                    QTimer::singleShot(this->command.delay.get() + this->command.duration.get(), this, SLOT(executeStop()));
            }

            this->executeTimer.start();
        }
    }
    else if (type == Playout::PlayoutType::PlayNow)
        executePlay();
    else if (type == Playout::PlayoutType::Clear)
        executeStop();
    else if (type == Playout::PlayoutType::ClearVideoLayer)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearChannel)
        executeClearChannel();

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownKeyerWidget::executeStop()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setKeyer(this->command.channel.get(), this->command.videolayer.get(), 0);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setKeyer(this->command.channel.get(), this->command.videolayer.get(), 0);
    }
}

void RundownKeyerWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->setKeyer(this->command.channel.get(), this->command.videolayer.get(), 1, this->command.getDefer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->setKeyer(this->command.channel.get(), this->command.videolayer.get(), 1, this->command.getDefer());
    }

    if (this->markUsedItems)
        setUsed(true);
}

void RundownKeyerWidget::executeClearVideolayer()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearMixerVideolayer(this->command.channel.get(), this->command.videolayer.get());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearMixerVideolayer(this->command.channel.get(), this->command.videolayer.get());
    }
}

void RundownKeyerWidget::executeClearChannel()
{
    this->executeTimer.stop();

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
}

void RundownKeyerWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownKeyerWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownKeyerWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownKeyerWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.allowGpi.get());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownKeyerWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownKeyerWidget::configureOscSubscriptions()
{
    if (!this->command.getAllowRemoteTriggering())
        return;

    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowControlSubscription != NULL)
        this->playNowControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

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

    QString updateControlFilter = Osc::ITEM_CONTROL_UPDATE_FILTER;
    updateControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

void RundownKeyerWidget::allowGpiChanged(bool allowGpi)
{
    Q_UNUSED(allowGpi);

    checkGpiConnection();
}

void RundownKeyerWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    Q_UNUSED(connected);
    Q_UNUSED(device);

    checkGpiConnection();
}

void RundownKeyerWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownKeyerWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    Q_UNUSED(device);

    checkDeviceConnection();
}

void RundownKeyerWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress())->getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}

void RundownKeyerWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Stop);
}

void RundownKeyerWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownKeyerWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::PlayNow);
}

void RundownKeyerWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Update);
}

void RundownKeyerWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Clear);
}

void RundownKeyerWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearVideoLayer);
}

void RundownKeyerWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearChannel);
}
