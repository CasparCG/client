#include "RundownVideoWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "GpiManager.h"
#include "EventManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"

#include <math.h>

#include <QtCore/QObject>
#include <QtCore/QFileInfo>

#include <QtGui/QPixmap>
#include <QtGui/QGraphicsOpacityEffect>

RundownVideoWidget::RundownVideoWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                       bool loaded, bool paused, bool playing, bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), loaded(loaded), paused(paused), playing(playing), inGroup(inGroup), compactView(compactView), color(color), model(model),
      fileModel(NULL), timeSubscription(NULL), frameSubscription(NULL), fpsSubscription(NULL), pathSubscription(NULL), pausedSubscription(NULL),
      loopSubscription(NULL), stopControlSubscription(NULL), playControlSubscription(NULL), playNowControlSubscription(NULL), loadControlSubscription(NULL),
      pauseControlSubscription(NULL), nextControlSubscription(NULL), updateControlSubscription(NULL), previewControlSubscription(NULL), clearControlSubscription(NULL),
      clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL), animation(NULL), reverseOscTime(false), sendAutoPlay(false), hasSentAutoPlay(false)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();
    this->markUsedItems = (DatabaseManager::getInstance().getConfigurationByName("MarkUsedItems").getValue() == "true") ? true : false;

    setThumbnail();
    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->command.setVideoName(this->model.getName());

    this->labelAutoPlay->setVisible(false);
    this->labelLoopOverlay->setVisible(false);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_MOVIE_COLOR));

    this->labelLabel->setText(this->model.getLabel());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.getChannel()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.getVideolayer()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeStartTimer.setSingleShot(true);
    this->executeStopTimer.setSingleShot(true);

    QObject::connect(&this->command, SIGNAL(channelChanged(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command, SIGNAL(videolayerChanged(int)), this, SLOT(videolayerChanged(int)));
    QObject::connect(&this->command, SIGNAL(delayChanged(int)), this, SLOT(delayChanged(int)));
    QObject::connect(&this->command, SIGNAL(allowGpiChanged(bool)), this, SLOT(allowGpiChanged(bool)));
    QObject::connect(&this->command, SIGNAL(loopChanged(bool)), this, SLOT(loopChanged(bool)));
    QObject::connect(&this->command, SIGNAL(autoPlayChanged(bool)), this, SLOT(autoPlayChanged(bool)));
    QObject::connect(&this->command, SIGNAL(remoteTriggerIdChanged(const QString&)), this, SLOT(remoteTriggerIdChanged(const QString&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(targetChanged(const TargetChangedEvent&)), this, SLOT(targetChanged(const TargetChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(labelChanged(const LabelChangedEvent&)), this, SLOT(labelChanged(const LabelChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(videolayerChanged(const VideolayerChangedEvent&)), this, SLOT(videolayerChanged(const VideolayerChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));

    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL)
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(connectionStateChanged(bool, GpiDevice*)), this, SLOT(gpiConnectionStateChanged(bool, GpiDevice*)));

    this->reverseOscTime = (DatabaseManager::getInstance().getConfigurationByName("ReverseOscTime").getValue() == "true") ? true : false;

    checkEmptyDevice();
    checkGpiConnection();
    checkDeviceConnection();

    configureOscSubscriptions();

    this->widgetOscTime->setStartTime(this->model.getTimecode(), this->reverseOscTime);
}

void RundownVideoWidget::videolayerChanged(const VideolayerChangedEvent& event)
{
    if (!this->active)
        return;

    configureOscSubscriptions();
}

void RundownVideoWidget::channelChanged(const ChannelChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    configureOscSubscriptions();
}

void RundownVideoWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel());
}

void RundownVideoWidget::targetChanged(const TargetChangedEvent& event)
{
    // This event is not for us.
    if (!this->active)
        return;

    this->model.setName(event.getTarget());
    this->command.setVideoName(event.getTarget());

    setThumbnail();
}

void RundownVideoWidget::deviceChanged(const DeviceChangedEvent& event)
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
    configureOscSubscriptions();
}

AbstractRundownWidget* RundownVideoWidget::clone()
{
    RundownVideoWidget* widget = new RundownVideoWidget(this->model, this->parentWidget(), this->color, this->active,
                                                        this->loaded, this->paused, this->playing, this->inGroup, this->compactView);

    VideoCommand* command = dynamic_cast<VideoCommand*>(widget->getCommand());
    command->setChannel(this->command.getChannel());
    command->setVideolayer(this->command.getVideolayer());
    command->setDelay(this->command.getDelay());
    command->setDuration(this->command.getDuration());
    command->setAllowGpi(this->command.getAllowGpi());
    command->setAllowRemoteTriggering(this->command.getAllowRemoteTriggering());
    command->setRemoteTriggerId(this->command.getRemoteTriggerId());
    command->setVideoName(this->command.getVideoName());
    command->setTransition(this->command.getTransition());
    command->setTransitionDuration(this->command.getTransitionDuration());
    command->setTween(this->command.getTween());
    command->setDirection(this->command.getDirection());
    command->setLoop(this->command.getLoop());
    command->setSeek(this->command.getSeek());
    command->setLength(this->command.getLength());
    command->setFreezeOnLoad(this->command.getFreezeOnLoad());
    command->setTriggerOnNext(this->command.getTriggerOnNext());
    command->setAutoPlay(this->command.getAutoPlay());

    return widget;
}

void RundownVideoWidget::setCompactView(bool compactView)
{
    if (compactView)
    {
        this->labelIcon->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelLoopOverlay->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
        this->labelThumbnail->setFixedSize(Rundown::COMPACT_THUMBNAIL_WIDTH, Rundown::COMPACT_THUMBNAIL_HEIGHT);
        this->labelAutoPlay->setFixedSize(Rundown::COMPACT_ICON_WIDTH, Rundown::COMPACT_ICON_HEIGHT);
    }
    else
    {
        this->labelIcon->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelLoopOverlay->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelGpiConnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelDisconnected->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
        this->labelThumbnail->setFixedSize(Rundown::DEFAULT_THUMBNAIL_WIDTH, Rundown::DEFAULT_THUMBNAIL_HEIGHT);
        this->labelAutoPlay->setFixedSize(Rundown::DEFAULT_ICON_WIDTH, Rundown::DEFAULT_ICON_HEIGHT);
    }

    this->widgetOscTime->setCompactView(compactView);

    this->compactView = compactView;
}

void RundownVideoWidget::readProperties(boost::property_tree::wptree& pt)
{
    setColor(QString::fromStdWString(pt.get(L"color", Color::DEFAULT_TRANSPARENT_COLOR.toStdWString())));
    setTimecode(QString::fromStdWString(pt.get(L"timecode", L"")));
}

void RundownVideoWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
    writer->writeTextElement("timecode", this->model.getTimecode());
}

bool RundownVideoWidget::isGroup() const
{
    return false;
}

bool RundownVideoWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownVideoWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownVideoWidget::getLibraryModel()
{
    return &this->model;
}

void RundownVideoWidget::setThumbnail()
{
    if (this->model.getType() == "AUDIO")
    {
        this->labelThumbnail->setVisible(false);
        return;
    }

    QString data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(this->model.getName(), this->model.getDeviceName()).getData();

    /*
    QString data = DatabaseManager::getInstance().getThumbnailById(this->model.getThumbnailId()).getData();
    if (data.isEmpty())
        data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(this->model.getName(), this->model.getDeviceName()).getData();
    */

    QImage image;
    image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");
    this->labelThumbnail->setPixmap(QPixmap::fromImage(image));

    bool displayThumbnailTooltip = (DatabaseManager::getInstance().getConfigurationByName("ShowThumbnailTooltip").getValue() == "true") ? true : false;
    if (displayThumbnailTooltip)
        this->labelThumbnail->setToolTip(QString("<img src=\"data:image/png;base64,%1 \"/>").arg(data));
}

void RundownVideoWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownVideoWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);

    if (!this->inGroup)
    {
        this->labelAutoPlay->setVisible(this->inGroup);
        this->command.setAutoPlay(this->inGroup);
    }
}

QString RundownVideoWidget::getColor() const
{
    return this->color;
}

void RundownVideoWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownVideoWidget::setTimecode(const QString& timecode)
{
    this->model.setTimecode(timecode);
    this->widgetOscTime->setStartTime(this->model.getTimecode(), this->reverseOscTime);
}

void RundownVideoWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: firebrick;");
    else
        this->labelDevice->setStyleSheet("");
}

void RundownVideoWidget::clearDelayedCommands()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownVideoWidget::setUsed(bool used)
{
    if (used)
    {
        if (this->graphicsEffect() == NULL)
        {
            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.25);

            this->setGraphicsEffect(effect);
        }
    }
    else
        this->setGraphicsEffect(NULL);
}

bool RundownVideoWidget::executeCommand(Playout::PlayoutType::Type type)
{
    if (type == Playout::PlayoutType::Stop)
        executeStop();
    else if (type == Playout::PlayoutType::Play)
    {
        if (this->command.getDelay() < 0)
            return true;

        if (this->command.getAutoPlay())
            executePlay();
        else if (!this->command.getTriggerOnNext() || type == Playout::PlayoutType::Update)
        {
            this->executeStartTimer.setInterval(0);
            this->executeStopTimer.setInterval(0);
            this->executeStartTimer.disconnect(); // Disconnect all events.
            this->executeStopTimer.disconnect(); // Disconnect all events.
            QObject::connect(&this->executeStartTimer, SIGNAL(timeout()), SLOT(executePlay()));
            QObject::connect(&this->executeStopTimer, SIGNAL(timeout()), SLOT(executeStop()));

            if (!this->model.getDeviceName().isEmpty()) // The user need to select a device.
            {
                if (this->delayType == Output::DEFAULT_DELAY_IN_FRAMES)
                {
                    const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(this->model.getDeviceName()).getChannelFormats().split(",");
                    if (this->command.getChannel() > channelFormats.count())
                        return true;

                    double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.getChannel() - 1]).getFramesPerSecond().toDouble();

                    int startDelay = floor(this->command.getDelay() * (1000 / framesPerSecond));
                    this->executeStartTimer.setInterval(startDelay);

                    if (this->command.getDuration() > 0)
                    {
                        int stopDelay = floor(this->command.getDuration() * (1000 / framesPerSecond));
                        this->executeStopTimer.setInterval(startDelay + stopDelay);
                    }
                }
                else if (this->delayType == Output::DEFAULT_DELAY_IN_MILLISECONDS)
                {
                    this->executeStartTimer.setInterval(this->command.getDelay());

                    if (this->command.getDuration() > 0)
                        this->executeStopTimer.setInterval(this->command.getDelay() + this->command.getDuration());
                }

                this->executeStartTimer.start();
                if (this->executeStopTimer.interval() > 0)
                    this->executeStopTimer.start();
            }
        }
    }
    else if (type == Playout::PlayoutType::PlayNow)
        executePlay();
    else if (type == Playout::PlayoutType::PauseResume)
        executePause();
    else if (type == Playout::PlayoutType::Load)
        executeLoad();
    else if (type == Playout::PlayoutType::Next)
    {
        if (this->command.getAutoPlay())
            executeNext();
        else if (this->command.getTriggerOnNext())
            executePlay();
    }
    else if (type == Playout::PlayoutType::Clear)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearVideoLayer)
        executeClearVideolayer();
    else if (type == Playout::PlayoutType::ClearChannel)
        executeClearChannel();
    else if (type == Playout::PlayoutType::Preview)
        executeLoadPreview();

    if (this->active)
        this->animation->start(1);

    return true;
}

void RundownVideoWidget::executeStop()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->stopVideo(this->command.getChannel(), this->command.getVideolayer());

        // Stop preview channels item.
        const DeviceModel deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel.getPreviewChannel() > 0)
            device->stopVideo(deviceModel.getPreviewChannel(), this->command.getVideolayer());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->stopVideo(this->command.getChannel(), this->command.getVideolayer());

            // Stop preview channels item.
            if (model.getPreviewChannel() > 0)
                deviceShadow->stopVideo(model.getPreviewChannel(), this->command.getVideolayer());
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
    this->sendAutoPlay= false;

    this->widgetOscTime->setPaused(this->paused);
    this->widgetOscTime->reset();

    this->hasSentAutoPlay = false;
}

void RundownVideoWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
        {
            device->playVideo(this->command.getChannel(), this->command.getVideolayer());
        }
        else
        {
            if (this->command.getAutoPlay())
            {
                device->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                  this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                  this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                  this->command.getLoop(), true);
            }
            else
            {
                device->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                  this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                  this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                  this->command.getLoop(), this->command.getAutoPlay());
            }
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
                deviceShadow->playVideo(this->command.getChannel(), this->command.getVideolayer());
            }
            else
            {
                if (this->command.getAutoPlay())
                {
                    deviceShadow->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                            this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                            this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                            this->command.getLoop(), true);
                }
                else
                {
                    deviceShadow->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                            this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                            this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                            this->command.getLoop(), this->command.getAutoPlay());
                }
            }
        }
    }

    if (this->markUsedItems)
        setUsed(true);

    this->paused = false;
    this->loaded = false;
    this->playing = true;
    this->hasSentAutoPlay = false;

    if (this->command.getAutoPlay())
        this->sendAutoPlay= true;
}

void RundownVideoWidget::executePause()
{
    if (!this->playing)
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->paused)
            device->resumeVideo(this->command.getChannel(), this->command.getVideolayer());
        else
            device->pauseVideo(this->command.getChannel(), this->command.getVideolayer());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            if (this->paused)
                deviceShadow->resumeVideo(this->command.getChannel(), this->command.getVideolayer());
            else
                deviceShadow->pauseVideo(this->command.getChannel(), this->command.getVideolayer());
        }
    }

    this->paused = !this->paused;

    this->widgetOscTime->setPaused(this->paused);
}

void RundownVideoWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->loadVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                          this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                          this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                          this->command.getLoop(), this->command.getFreezeOnLoad(), false);
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->loadVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                    this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                    this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                    this->command.getLoop(), this->command.getFreezeOnLoad(), false);
        }
    }

    this->loaded = true;
    this->paused = false;
    this->playing = false;
    this->sendAutoPlay= false;
}

void RundownVideoWidget::executeLoadPreview()
{
    const DeviceModel deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
    if (deviceModel.getPreviewChannel() > 0)
    {
        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (device != NULL && device->isConnected())
        {
            device->loadVideo(deviceModel.getPreviewChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                              this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                              this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                              this->command.getLoop(), true, false);
        }
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        if (model.getPreviewChannel() > 0)
        {
            const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
            if (deviceShadow != NULL && deviceShadow->isConnected())
            {
                deviceShadow->loadVideo(model.getPreviewChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                        this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                        this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                        this->command.getLoop(), true, false);
            }
        }
    }
}

void RundownVideoWidget::executeNext()
{
    if (this->command.getAutoPlay())
    {
        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (device != NULL && device->isConnected())
        {
            device->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                              this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                              this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                              this->command.getLoop(), false);

        }

        foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
        {
            if (model.getShadow() == "No")
                continue;

            const QSharedPointer<CasparDevice>  deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
            if (deviceShadow != NULL && deviceShadow->isConnected())
            {
                deviceShadow->playVideo(this->command.getChannel(), this->command.getVideolayer(), this->command.getVideoName(),
                                        this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                        this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                        this->command.getLoop(), false);
            }
        }

        this->paused = false;
        this->loaded = false;
        this->playing = true;

        if (this->command.getAutoPlay())
            this->sendAutoPlay= true;
    }
}

void RundownVideoWidget::executeClearVideolayer()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());

        // Clear preview channels videolayer.
        const DeviceModel deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel.getPreviewChannel() > 0)
            device->clearVideolayer(deviceModel.getPreviewChannel(), this->command.getVideolayer());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearVideolayer(this->command.getChannel(), this->command.getVideolayer());

            // Clear preview channels videolayer.
            if (model.getPreviewChannel() > 0)
                deviceShadow->clearVideolayer(model.getPreviewChannel(), this->command.getVideolayer());
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
    this->sendAutoPlay= false;

    this->widgetOscTime->setPaused(this->paused);
    this->widgetOscTime->reset();

    this->hasSentAutoPlay = false;
}

void RundownVideoWidget::executeClearChannel()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.getChannel());
        device->clearMixerChannel(this->command.getChannel());

        // Clear preview channel.
        const DeviceModel deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel.getPreviewChannel() > 0)
        {
            device->clearChannel(deviceModel.getPreviewChannel());
            device->clearMixerChannel(deviceModel.getPreviewChannel());
        }
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

            // Clear preview channel.
            if (model.getPreviewChannel() > 0)
            {
                deviceShadow->clearChannel(model.getPreviewChannel());
                deviceShadow->clearMixerChannel(model.getPreviewChannel());
            }
        }
    }

    this->paused = false;
    this->loaded = false;
    this->playing = false;
    this->sendAutoPlay= false;

    this->widgetOscTime->setPaused(this->paused);
    this->widgetOscTime->reset();

    this->hasSentAutoPlay = false;
}

void RundownVideoWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownVideoWidget::checkDeviceConnection()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device == NULL)
        this->labelDisconnected->setVisible(true);
    else
    {
        this->widgetOscTime->reset();
        this->labelDisconnected->setVisible(!device->isConnected());
    }
}

void RundownVideoWidget::configureOscSubscriptions()
{
    if (DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName()) == NULL)
        return;

    if (this->timeSubscription != NULL)
        this->timeSubscription->disconnect(); // Disconnect all events.

    if (this->frameSubscription != NULL)
        this->frameSubscription->disconnect(); // Disconnect all events.

    if (this->fpsSubscription != NULL)
        this->fpsSubscription->disconnect(); // Disconnect all events.

    if (this->pathSubscription != NULL)
        this->pathSubscription->disconnect(); // Disconnect all events.

    if (this->pausedSubscription != NULL)
        this->pausedSubscription->disconnect(); // Disconnect all events.

    if (this->loopSubscription != NULL)
        this->loopSubscription->disconnect(); // Disconnect all events.

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

    if (this->nextControlSubscription != NULL)
        this->nextControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

    if (this->previewControlSubscription != NULL)
        this->previewControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.

    QString timeFilter = Osc::DEFAULT_TIME_FILTER;
    timeFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
              .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
              .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->timeSubscription = new OscSubscription(timeFilter, this);
    QObject::connect(this->timeSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(timeSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString frameFilter = Osc::DEFAULT_FRAME_FILTER;
    frameFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
               .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
               .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->frameSubscription = new OscSubscription(frameFilter, this);
    QObject::connect(this->frameSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(frameSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString fpsFilter = Osc::DEFAULT_FPS_FILTER;
    fpsFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
             .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
             .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->fpsSubscription = new OscSubscription(fpsFilter, this);
    QObject::connect(this->fpsSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(fpsSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pathFilter = Osc::DEFAULT_PATH_FILTER;
    pathFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
              .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
              .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->pathSubscription = new OscSubscription(pathFilter, this);
    QObject::connect(this->pathSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pathSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pausedFilter = Osc::DEFAULT_PAUSED_FILTER;
    pausedFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
                .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
                .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->pausedSubscription = new OscSubscription(pausedFilter, this);
    QObject::connect(this->pausedSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pausedSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loopFilter = Osc::DEFAULT_LOOP_FILTER;
    loopFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
              .replace("#CHANNEL#", QString("%1").arg(this->command.getChannel()))
              .replace("#VIDEOLAYER#", QString("%1").arg(this->command.getVideolayer()));
    this->loopSubscription = new OscSubscription(loopFilter, this);
    QObject::connect(this->loopSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loopSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

    QString playNowControlFilter = Osc::DEFAULT_PLAYNOW_CONTROL_FILTER;
    playNowControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->playNowControlSubscription = new OscSubscription(playNowControlFilter, this);
    QObject::connect(this->playNowControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::DEFAULT_LOAD_CONTROL_FILTER;
    loadControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pauseControlFilter = Osc::DEFAULT_PAUSE_CONTROL_FILTER;
    pauseControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->pauseControlSubscription = new OscSubscription(pauseControlFilter, this);
    QObject::connect(this->pauseControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

    QString previewControlFilter = Osc::DEFAULT_PREVIEW_CONTROL_FILTER;
    previewControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->previewControlSubscription = new OscSubscription(previewControlFilter, this);
    QObject::connect(this->previewControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(previewControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

void RundownVideoWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));

    configureOscSubscriptions();
}

void RundownVideoWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));

    configureOscSubscriptions();
}

void RundownVideoWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownVideoWidget::allowGpiChanged(bool allowGpi)
{
    checkGpiConnection();
}

void RundownVideoWidget::loopChanged(bool loop)
{
    this->labelLoopOverlay->setVisible(loop);
}

void RundownVideoWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    checkGpiConnection();
}

void RundownVideoWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownVideoWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    checkDeviceConnection();
}

void RundownVideoWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress()).getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
    configureOscSubscriptions();
}

void RundownVideoWidget::timeSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    //qDebug() << predicate << " " << arguments;

    if (this->fileModel != NULL)
    {
        delete this->fileModel;
        this->fileModel = NULL;
    }

    this->fileModel = new OscFileModel();
    this->fileModel->setTime(arguments.at(0).toDouble());
    this->fileModel->setTotalTime(arguments.at(1).toDouble());
}

void RundownVideoWidget::frameSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    //qDebug() << predicate << " " << arguments;

    if (this->fileModel == NULL)
        return;

    this->fileModel->setFrame(arguments.at(0).toInt());
    this->fileModel->setTotalFrames(arguments.at(1).toInt());
}

void RundownVideoWidget::fpsSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    //qDebug() << predicate << " " << arguments;

    if (this->fileModel == NULL)
        return;

    this->fileModel->setFramesPerSecond(arguments.at(0).toInt());
}

void RundownVideoWidget::pathSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    //qDebug() << predicate << " " << arguments << "\n";

    QString name = arguments.at(0).toString();
    name.remove(name.lastIndexOf('.'), name.length()); // Remove extension.

    if (this->model.getName().toLower() != name.toLower())
        return; // Wrong file.

    if (this->fileModel == NULL)
        return;

    this->fileModel->setPath(arguments.at(0).toString());

    this->widgetOscTime->setProgress(this->fileModel->getFrame());

    if (this->reverseOscTime && this->fileModel->getFrame() > 0)
        this->widgetOscTime->setTime(this->fileModel->getTotalFrames() - this->fileModel->getFrame());
    else
        this->widgetOscTime->setTime(this->fileModel->getFrame());

    if (this->command.getSeek() == 0 && this->command.getLength() == 0)
        this->widgetOscTime->setInOutTime(0, this->fileModel->getTotalFrames());
    else
        this->widgetOscTime->setInOutTime(this->command.getSeek(), this->command.getLength());

    this->widgetOscTime->setFramesPerSecond(this->fileModel->getFramesPerSecond());

    if (this->sendAutoPlay && !this->hasSentAutoPlay)
    {
        EventManager::getInstance().fireAutoPlayRundownItemEvent(AutoPlayRundownItemEvent(this));

        this->sendAutoPlay = false;
        this->hasSentAutoPlay = true;

        qDebug() << "RundownVideoWidget::pathSubscriptionReceived: Dispatched AutoPlay event";
    }

    this->playing = true;
}

void RundownVideoWidget::pausedSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    this->widgetOscTime->setPaused(arguments.at(0).toBool());
}

void RundownVideoWidget::loopSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    this->widgetOscTime->setLoop(arguments.at(0).toBool());
}

void RundownVideoWidget::autoPlayChanged(bool autoPlay)
{
    this->labelAutoPlay->setVisible(autoPlay);
}

void RundownVideoWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Stop);
}

void RundownVideoWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownVideoWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::PlayNow);
}

void RundownVideoWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Load);
}

void RundownVideoWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::PauseResume);
}

void RundownVideoWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Next);
}

void RundownVideoWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Update);
}

void RundownVideoWidget::previewControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Preview);
}

void RundownVideoWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::Clear);
}

void RundownVideoWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::ClearVideoLayer);
}

void RundownVideoWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0] == 1)
        executeCommand(Playout::PlayoutType::ClearChannel);
}
