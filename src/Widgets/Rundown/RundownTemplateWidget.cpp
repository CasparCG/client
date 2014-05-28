#include "RundownTemplateWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Animations/ActiveAnimation.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/Inspector/AddTemplateDataEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

RundownTemplateWidget::RundownTemplateWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                             bool loaded, bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), loaded(loaded), inGroup(inGroup), compactView(compactView), color(color), model(model), stopControlSubscription(NULL),
      playControlSubscription(NULL), loadControlSubscription(NULL), nextControlSubscription(NULL), updateControlSubscription(NULL),
      invokeControlSubscription(NULL), clearControlSubscription(NULL), clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL)
{
    setupUi(this);
    setAcceptDrops(true);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();

    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->command.setTemplateName(this->model.getName());

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_TEMPLATE_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelFlashlayer->setText(QString("Flash layer: %1").arg(this->command.getFlashlayer()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeTimer.setSingleShot(true);

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(flashlayerChanged(int)), this, SLOT(flashlayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(remoteTriggerIdChanged(const QString&)), this, SLOT(remoteTriggerIdChanged(const QString&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(targetChanged(const TargetChangedEvent&)), this, SLOT(targetChanged(const TargetChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(labelChanged(const LabelChangedEvent&)), this, SLOT(labelChanged(const LabelChangedEvent&)));

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    configureOscSubscriptions();

    this->installEventFilter(this);
}

bool RundownTemplateWidget::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonDblClick)
        EventManager::getInstance().fireShowAddTemplateDataDialogEvent(ShowAddTemplateDataDialogEvent());

    return QObject::eventFilter(object, event);
}

void RundownTemplateWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel());
}

void RundownTemplateWidget::targetChanged(const TargetChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    this->model.setName(event.getTarget());
    this->command.setTemplateName(event.getTarget());
}

void RundownTemplateWidget::deviceChanged(const DeviceChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
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

void RundownTemplateWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (!this->active)
        return;

    if (event->mimeData()->hasFormat("application/library-dataitem"))
        event->acceptProposedAction();
}

void RundownTemplateWidget::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->hasFormat("application/library-dataitem"))
    {
        QString dndData = QString::fromUtf8(event->mimeData()->data("application/library-dataitem"));
        if (dndData.startsWith("<treeWidgetData>"))
        {
            QStringList dataSplit = dndData.split(",");
            EventManager::getInstance().fireAddTemplateDataEvent(AddTemplateDataEvent(dataSplit.at(1), true));
        }
    }
}

AbstractRundownWidget* RundownTemplateWidget::clone()
{
    RundownTemplateWidget* widget = new RundownTemplateWidget(this->model, this->parentWidget(), this->color, this->active,
                                                              this->loaded, this->inGroup, this->compactView);

    TemplateCommand* command = dynamic_cast<TemplateCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setDuration(this->command.getDuration());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setRemoteTriggerId(this->command.getRemoteTriggerId());
    command->setFlashlayer(this->command.getFlashlayer());
    command->setInvoke(this->command.getInvoke());
    command->setTemplateName(this->command.getTemplateName());
    command->setTemplateDataModels(this->command.getTemplateDataModels());
    command->setUseStoredData(this->command.getUseStoredData());
    command->setUseUppercaseData(this->command.getUseUppercaseData());

    return widget;
}

void RundownTemplateWidget::setCompactView(bool compactView)
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

void RundownTemplateWidget::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void RundownTemplateWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
}

bool RundownTemplateWidget::isGroup() const
{
    return false;
}

bool RundownTemplateWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownTemplateWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownTemplateWidget::getLibraryModel()
{
    return &this->model;
}

void RundownTemplateWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownTemplateWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);
}

void RundownTemplateWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownTemplateWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: firebrick;");
    else
        this->labelDevice->setStyleSheet("");
}

void RundownTemplateWidget::clearDelayedCommands()
{
    this->executeTimer.stop();
}

bool RundownTemplateWidget::executeCommand(Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        executeStop();
    else if (type == Playout::PlayoutType::Play)
    {
        this->executeTimer.disconnect(); // Disconnect all events.
        QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executePlay()));

        if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
            if (this->command.getChannel() > channelFormats.count())
                return true;

            double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.getChannel() - 1]).getFramesPerSecond().toDouble();

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
    else if (type == Playout::PlayoutType::PlayNow)
        executePlay();
    else if (type == Playout::PlayoutType::Update)
    {
        this->executeTimer.disconnect(); // Disconnect all events.
        QObject::connect(&this->executeTimer, SIGNAL(timeout()), SLOT(executeUpdate()));

        if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
        {
            if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
            {
                const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
                if (this->command.getChannel() > channelFormats.count())
                    return true;

                double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.getChannel() - 1]).getFramesPerSecond().toDouble();

                this->executeTimer.setInterval(floor(this->command.getDelay() * (1000 / framesPerSecond)));
            }
            else if (this->delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
            {
                this->executeTimer.setInterval(this->command.getDelay());
            }

            this->executeTimer.start();
        }
    }
    else if (type == Playout::PlayoutType::Load)
        executeLoad();
    else if (type == Playout::PlayoutType::Next)
        executeNext();
    else if (type == Playout::PlayoutType::Invoke)
        executeInvoke();
    else if (type == Playout::PlayoutType::Clear)
        executeClear();
    else if (type == Playout::PlayoutType::ClearVideolayer)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearChannel)
        executeClearChannel();

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownTemplateWidget::executeStop()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->stopTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->stopTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
        {
            device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
        }
        else
        {
            if (this->command.getTemplateData().isEmpty())
                device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                     this->command.getTemplateName());
            else
                device->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                     this->command.getTemplateName(), this->command.getTemplateData());
        }
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->loaded)
            {
                deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
            }
            else
            {
                if (this->command.getTemplateData().isEmpty())
                    deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                               this->command.getTemplateName());
                else
                    deviceShadow->playTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                               this->command.getTemplateName(), this->command.getTemplateData());
            }
        }
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->command.getTemplateData().isEmpty())
            device->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                this->command.getTemplateName(), false);
        else
            device->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                this->command.getTemplateName(), false, this->command.getTemplateData());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->command.getTemplateData().isEmpty())
                deviceShadow->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                          this->command.getTemplateName(), false);
            else
                deviceShadow->addTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer(),
                                          this->command.getTemplateName(), false, this->command.getTemplateData());
        }
    }

    this->loaded = true;
}

void RundownTemplateWidget::executeNext()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->nextTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->nextTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }
}

void RundownTemplateWidget::executeUpdate()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->updateTemplate(this->command.getChannel(), this->command.getVideolayer(),
                               this->command.getFlashlayer(), this->command.getTemplateData());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->updateTemplate(this->command.getChannel(), this->command.getVideolayer(),
                                         this->command.getFlashlayer(), this->command.getTemplateData());
        }
    }
}

void RundownTemplateWidget::executeInvoke()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->invokeTemplate(this->command.getChannel(), this->command.getVideolayer(),
                               this->command.getFlashlayer(), this->command.getInvoke());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->invokeTemplate(this->command.getChannel(), this->command.getVideolayer(),
                                         this->command.getFlashlayer(), this->command.getInvoke());
    }
}

void RundownTemplateWidget::executeClear()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->removeTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->removeTemplate(this->command.getChannel(), this->command.getVideolayer(), this->command.getFlashlayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeClearVideolayer()
{
    this->executeTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
        device->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
            deviceShadow->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());
    }

    this->loaded = false;
}

void RundownTemplateWidget::executeClearChannel()
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

    this->loaded = false;
}

void RundownTemplateWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));
}

void RundownTemplateWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));
}

void RundownTemplateWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownTemplateWidget::flashlayerChanged(int flashlayer)
{
    this->labelFlashlayer->setText(QString("Flash layer: %1").arg(flashlayer));
}

void RundownTemplateWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownTemplateWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
        this->labelDisconnected->setVisible(!device->isConnected());
}

void RundownTemplateWidget::configureOscSubscriptions()
{
    if (DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName()) == NULL)
        return;

    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->loadControlSubscription != NULL)
        this->loadControlSubscription->disconnect(); // Disconnect all events.

    if (this->nextControlSubscription != NULL)
        this->nextControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

    if (this->invokeControlSubscription != NULL)
        this->invokeControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.

    QString stopControlFilter = Osc::DEFAULT_STOP_CONTROL_FILTER;
    stopControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->stopControlSubscription = new OscSubscription(stopControlFilter, this);
    QObject::connect(this->stopControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(stopControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playControlFilter = Osc::DEFAULT_PLAY_CONTROL_FILTER;
    playControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::DEFAULT_LOAD_CONTROL_FILTER;
    loadControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString nextControlFilter = Osc::DEFAULT_NEXT_CONTROL_FILTER;
    nextControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->nextControlSubscription = new OscSubscription(nextControlFilter, this);
    QObject::connect(this->nextControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(nextControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString updateControlFilter = Osc::DEFAULT_UPDATE_CONTROL_FILTER;
    updateControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString invokeControlFilter = Osc::DEFAULT_INVOKE_CONTROL_FILTER;
    invokeControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->invokeControlSubscription = new OscSubscription(invokeControlFilter, this);
    QObject::connect(this->invokeControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearControlFilter = Osc::DEFAULT_CLEAR_CONTROL_FILTER;
    clearControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearControlSubscription = new OscSubscription(clearControlFilter, this);
    QObject::connect(this->clearControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearVideolayerControlFilter = Osc::DEFAULT_CLEAR_VIDEOLAYER_CONTROL_FILTER;
    clearVideolayerControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearVideolayerControlSubscription = new OscSubscription(clearVideolayerControlFilter, this);
    QObject::connect(this->clearVideolayerControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearChannelControlFilter = Osc::DEFAULT_CLEAR_CHANNEL_CONTROL_FILTER;
    clearChannelControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->clearChannelControlSubscription = new OscSubscription(clearChannelControlFilter, this);
    QObject::connect(this->clearChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownTemplateWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownTemplateWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownTemplateWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownTemplateWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownTemplateWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
}

void RundownTemplateWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Stop);
}

void RundownTemplateWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownTemplateWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Load);
}

void RundownTemplateWidget::invokeControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Invoke);
}

void RundownTemplateWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Next);
}

void RundownTemplateWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Update);
}

void RundownTemplateWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Clear);
}

void RundownTemplateWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::ClearVideolayer);
}

void RundownTemplateWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::ClearChannel);
}
