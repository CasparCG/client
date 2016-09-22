#include "RundownMovieWidget.h"

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

#include <QtWidgets/QGraphicsOpacityEffect>

RundownMovieWidget::RundownMovieWidget(const LibraryModel& model, QWidget* parent, const QString& color, bool active,
                                       bool loaded, bool paused, bool playing, bool inGroup, bool compactView)
    : QWidget(parent),
      active(active), loaded(loaded), paused(paused), playing(playing), inGroup(inGroup), compactView(compactView), color(color), model(model),
      reverseOscTime(false), sendAutoPlay(false), hasSentAutoPlay(false), useFreezeOnLoad(false), frameSubscription(NULL), fpsSubscription(NULL),
      pathSubscription(NULL), pausedSubscription(NULL), loopSubscription(NULL), stopControlSubscription(NULL), playControlSubscription(NULL),
      playNowControlSubscription(NULL), loadControlSubscription(NULL), pauseControlSubscription(NULL), nextControlSubscription(NULL), updateControlSubscription(NULL),
      previewControlSubscription(NULL), clearControlSubscription(NULL), clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL)
{
    setupUi(this);

    this->animation = new ActiveAnimation(this->labelActiveColor);

    this->delayType = DatabaseManager::getInstance().getConfigurationByName("DelayType").getValue();
    this->markUsedItems = (DatabaseManager::getInstance().getConfigurationByName("MarkUsedItems").getValue() == "true") ? true : false;
    this->useFreezeOnLoad = (DatabaseManager::getInstance().getConfigurationByName("UseFreezeOnLoad").getValue() == "true") ? true : false;

    setThumbnail();
    setColor(this->color);
    setActive(this->active);
    setCompactView(this->compactView);

    this->command.setVideoName(this->model.getName());
    this->command.setFreezeOnLoad(this->useFreezeOnLoad);

    this->labelAutoPlay->setVisible(false);
    this->labelLoopOverlay->setVisible(false);

    this->labelGroupColor->setVisible(this->inGroup);
    this->labelGroupColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_GROUP_COLOR));
    this->labelColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_MOVIE_COLOR));

    this->labelLabel->setText(this->model.getLabel().split('/').last());
    this->labelChannel->setText(QString("Channel: %1").arg(this->command.channel.get()));
    this->labelVideolayer->setText(QString("Video layer: %1").arg(this->command.videolayer.get()));
    this->labelDelay->setText(QString("Delay: %1").arg(this->command.getDelay()));
    this->labelDevice->setText(QString("Server: %1").arg(this->model.getDeviceName()));

    this->executeStartTimer.setSingleShot(true);
    this->executeStopTimer.setSingleShot(true);

    QObject::connect(&this->command.channel, SIGNAL(changed(int)), this, SLOT(channelChanged(int)));
    QObject::connect(&this->command.videolayer, SIGNAL(changed(int)), this, SLOT(videolayerChanged(int)));
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

void RundownMovieWidget::videolayerChanged(const VideolayerChangedEvent& event)
{
    Q_UNUSED(event);

    if (!this->selected)
        return;

    configureOscSubscriptions();
}

void RundownMovieWidget::channelChanged(const ChannelChangedEvent& event)
{
    Q_UNUSED(event);

    // This event is not for us.
    if (!this->selected)
        return;

    configureOscSubscriptions();
}

void RundownMovieWidget::labelChanged(const LabelChangedEvent& event)
{
    // This event is not for us.
    if (!this->selected)
        return;

    this->model.setLabel(event.getLabel());

    this->labelLabel->setText(this->model.getLabel().split('/').last());
}

void RundownMovieWidget::targetChanged(const TargetChangedEvent& event)
{
    // This event is not for us.
    if (!this->selected)
        return;

    this->model.setName(event.getTarget());
    this->command.setVideoName(event.getTarget());

    setThumbnail();
}

void RundownMovieWidget::deviceChanged(const DeviceChangedEvent& event)
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
    configureOscSubscriptions();
}

AbstractRundownWidget* RundownMovieWidget::clone()
{
    RundownMovieWidget* widget = new RundownMovieWidget(this->model, this->parentWidget(), this->color, this->active,
                                                        this->loaded, this->paused, this->playing, this->inGroup, this->compactView);

    MovieCommand* command = dynamic_cast<MovieCommand*>(widget->getCommand());
    command->channel.set(this->command.channel.get());
    command->videolayer.set(this->command.videolayer.get());
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

void RundownMovieWidget::setCompactView(bool compactView)
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

void RundownMovieWidget::readProperties(boost::property_tree::wptree& pt)
{
    setColor(QString::fromStdWString(pt.get(L"color", Color::DEFAULT_TRANSPARENT_COLOR.toStdWString())));
    setTimecode(QString::fromStdWString(pt.get(L"timecode", L"")));
}

void RundownMovieWidget::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("color", this->color);
    writer->writeTextElement("timecode", this->model.getTimecode());
}

bool RundownMovieWidget::isGroup() const
{
    return false;
}

bool RundownMovieWidget::isInGroup() const
{
    return this->inGroup;
}

AbstractCommand* RundownMovieWidget::getCommand()
{
    return &this->command;
}

LibraryModel* RundownMovieWidget::getLibraryModel()
{
    return &this->model;
}

void RundownMovieWidget::setThumbnail()
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
    image.loadFromData(QByteArray::fromBase64(data.toLatin1()), "PNG");
    this->labelThumbnail->setPixmap(QPixmap::fromImage(image));

    bool displayThumbnailTooltip = (DatabaseManager::getInstance().getConfigurationByName("ShowThumbnailTooltip").getValue() == "true") ? true : false;
    if (displayThumbnailTooltip)
        this->labelThumbnail->setToolTip(QString("<img src=\"data:image/png;base64,%1 \"/>").arg(data));
}

void RundownMovieWidget::setSelected(bool selected)
{
    this->selected = selected;
}

void RundownMovieWidget::setActive(bool active)
{
    this->active = active;

    this->animation->stop();

    if (this->active)
        this->labelActiveColor->setStyleSheet(QString("background-color: %1;").arg(Color::DEFAULT_ACTIVE_COLOR));
    else
        this->labelActiveColor->setStyleSheet("");
}

void RundownMovieWidget::setInGroup(bool inGroup)
{
    this->inGroup = inGroup;
    this->labelGroupColor->setVisible(this->inGroup);

    if (!this->inGroup)
    {
        this->labelAutoPlay->setVisible(this->inGroup);
        this->command.setAutoPlay(this->inGroup);
    }
}

QString RundownMovieWidget::getColor() const
{
    return this->color;
}

void RundownMovieWidget::setColor(const QString& color)
{
    this->color = color;
    this->setStyleSheet(QString("#frameItem, #frameStatus { background-color: %1; }").arg(color));
}

void RundownMovieWidget::setTimecode(const QString& timecode)
{
    this->model.setTimecode(timecode);
    this->widgetOscTime->setStartTime(this->model.getTimecode(), this->reverseOscTime);
}

void RundownMovieWidget::checkEmptyDevice()
{
    if (this->labelDevice->text() == "Device: ")
        this->labelDevice->setStyleSheet("color: firebrick;");
    else
        this->labelDevice->setStyleSheet("");
}

void RundownMovieWidget::clearDelayedCommands()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    this->paused = false;
    this->loaded = false;
    this->playing = false;
}

void RundownMovieWidget::setUsed(bool used)
{
    if (used)
    {
        if (this->frameItem->graphicsEffect() == NULL || this->labelThumbnail->graphicsEffect() == NULL || this->widgetOscTime->graphicsEffect() == NULL)
        {
            QGraphicsOpacityEffect* frameItemEffect = new QGraphicsOpacityEffect(this);
            frameItemEffect->setOpacity(0.25);

            QGraphicsOpacityEffect* labelThumbnailEffect = new QGraphicsOpacityEffect(this);
            labelThumbnailEffect->setOpacity(0.25);

            QGraphicsOpacityEffect* widgetOscTimeEffect = new QGraphicsOpacityEffect(this);
            widgetOscTimeEffect->setOpacity(0.25);

            this->frameItem->setGraphicsEffect(frameItemEffect);
            this->labelThumbnail->setGraphicsEffect(labelThumbnailEffect);
            this->widgetOscTime->setGraphicsEffect(widgetOscTimeEffect);
        }
    }
    else
    {
        this->frameItem->setGraphicsEffect(NULL);
        this->labelThumbnail->setGraphicsEffect(NULL);
        this->widgetOscTime->setGraphicsEffect(NULL);
    }
}

bool RundownMovieWidget::executeCommand(Playout::PlayoutType type)
{
    if (type == Playout::PlayoutType::Stop)
        executeStop();
    else if ((type == Playout::PlayoutType::Play && !this->command.getTriggerOnNext()) || type == Playout::PlayoutType::Update)
    {
        if (this->command.getDelay() < 0)
            return true;

        if (this->command.getAutoPlay())
            executePlay();
        else
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
                    if (this->command.channel.get() > channelFormats.count())
                        return true;

                    double framesPerSecond = DatabaseManager::getInstance().getFormat(channelFormats[this->command.channel.get() - 1]).getFramesPerSecond().toDouble();

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

void RundownMovieWidget::executeStop()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->stop(this->command.channel.get(), this->command.videolayer.get());

        // Stop preview channels item.
        const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel != NULL && deviceModel->getPreviewChannel() > 0)
            device->stop(deviceModel->getPreviewChannel(), this->command.videolayer.get());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->stop(this->command.channel.get(), this->command.videolayer.get());

            // Stop preview channels item.
            if (model.getPreviewChannel() > 0)
                deviceShadow->stop(model.getPreviewChannel(), this->command.videolayer.get());
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

void RundownMovieWidget::executePlay()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        if (this->loaded)
        {
            device->play(this->command.channel.get(), this->command.videolayer.get());
        }
        else
        {
            if (this->command.getAutoPlay())
            {
                device->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
                                  this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                  this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                  this->command.getLoop(), true);
            }
            else
            {
                device->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
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
                deviceShadow->play(this->command.channel.get(), this->command.videolayer.get());
            }
            else
            {
                if (this->command.getAutoPlay())
                {
                    deviceShadow->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
                                            this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                            this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                            this->command.getLoop(), true);
                }
                else
                {
                    deviceShadow->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
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

void RundownMovieWidget::executePause()
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

    this->widgetOscTime->setPaused(this->paused);
}

void RundownMovieWidget::executeLoad()
{
    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->loadMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
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
            deviceShadow->loadMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
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

void RundownMovieWidget::executeLoadPreview()
{
    const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
    if (deviceModel != NULL && deviceModel->getPreviewChannel() > 0)
    {
        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (device != NULL && device->isConnected())
        {
            device->loadMovie(deviceModel->getPreviewChannel(), this->command.videolayer.get(), this->command.getVideoName(),
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
            const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
            if (deviceShadow != NULL && deviceShadow->isConnected())
            {
                deviceShadow->loadMovie(model.getPreviewChannel(), this->command.videolayer.get(), this->command.getVideoName(),
                                        this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                                        this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                                        this->command.getLoop(), true, false);
            }
        }
    }
}

void RundownMovieWidget::executeNext()
{
    if (this->command.getAutoPlay())
    {
        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
        if (device != NULL && device->isConnected())
        {
            device->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
                              this->command.getTransition(), this->command.getTransitionDuration(), this->command.getTween(),
                              this->command.getDirection(), this->command.getSeek(), this->command.getLength(),
                              this->command.getLoop(), false);

        }

        foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
        {
            if (model.getShadow() == "No")
                continue;

            const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
            if (deviceShadow != NULL && deviceShadow->isConnected())
            {
                deviceShadow->playMovie(this->command.channel.get(), this->command.videolayer.get(), this->command.getVideoName(),
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

void RundownMovieWidget::executeClearVideolayer()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearVideolayer(this->command.channel.get(), this->command.videolayer.get());

        // Clear preview channels videolayer.
        const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel != NULL && deviceModel->getPreviewChannel() > 0)
            device->clearVideolayer(deviceModel->getPreviewChannel(), this->command.videolayer.get());
    }

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "No")
            continue;

        const QSharedPointer<CasparDevice> deviceShadow = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (deviceShadow != NULL && deviceShadow->isConnected())
        {
            deviceShadow->clearVideolayer(this->command.channel.get(), this->command.videolayer.get());

            // Clear preview channels videolayer.
            if (model.getPreviewChannel() > 0)
                deviceShadow->clearVideolayer(model.getPreviewChannel(), this->command.videolayer.get());
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

void RundownMovieWidget::executeClearChannel()
{
    this->executeStartTimer.stop();
    this->executeStopTimer.stop();

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName());
    if (device != NULL && device->isConnected())
    {
        device->clearChannel(this->command.channel.get());
        device->clearMixerChannel(this->command.channel.get());

        // Clear preview channel.
        const QSharedPointer<DeviceModel> deviceModel = DeviceManager::getInstance().getDeviceModelByName(this->model.getDeviceName());
        if (deviceModel != NULL && deviceModel->getPreviewChannel() > 0)
        {
            device->clearChannel(deviceModel->getPreviewChannel());
            device->clearMixerChannel(deviceModel->getPreviewChannel());
        }
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

void RundownMovieWidget::checkGpiConnection()
{
    this->labelGpiConnected->setVisible(this->command.getAllowGpi());

    if (GpiManager::getInstance().getGpiDevice()->isConnected())
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiConnected.png"));
    else
        this->labelGpiConnected->setPixmap(QPixmap(":/Graphics/Images/GpiDisconnected.png"));
}

void RundownMovieWidget::checkDeviceConnection()
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

void RundownMovieWidget::configureOscSubscriptions()
{
    if (DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName()) == NULL)
            return;

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

    QString frameFilter = Osc::VIDEOLAYER_FRAME_FILTER;
    frameFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
               .replace("#CHANNEL#", QString("%1").arg(this->command.channel.get()))
               .replace("#VIDEOLAYER#", QString("%1").arg(this->command.videolayer.get()));
    this->frameSubscription = new OscSubscription(frameFilter, this);
    QObject::connect(this->frameSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(frameSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString fpsFilter = Osc::VIDEOLAYER_FPS_FILTER;
    fpsFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
             .replace("#CHANNEL#", QString("%1").arg(this->command.channel.get()))
             .replace("#VIDEOLAYER#", QString("%1").arg(this->command.videolayer.get()));
    this->fpsSubscription = new OscSubscription(fpsFilter, this);
    QObject::connect(this->fpsSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(fpsSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pathFilter = Osc::VIDEOLAYER_PATH_FILTER;
    pathFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
              .replace("#CHANNEL#", QString("%1").arg(this->command.channel.get()))
              .replace("#VIDEOLAYER#", QString("%1").arg(this->command.videolayer.get()));
    this->pathSubscription = new OscSubscription(pathFilter, this);
    QObject::connect(this->pathSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pathSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pausedFilter = Osc::VIDEOLAYER_PAUSED_FILTER;
    pausedFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
                .replace("#CHANNEL#", QString("%1").arg(this->command.channel.get()))
                .replace("#VIDEOLAYER#", QString("%1").arg(this->command.videolayer.get()));
    this->pausedSubscription = new OscSubscription(pausedFilter, this);
    QObject::connect(this->pausedSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(pausedSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loopFilter = Osc::VIDEOLAYER_LOOP_FILTER;
    loopFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model.getDeviceName())->resolveIpAddress()))
              .replace("#CHANNEL#", QString("%1").arg(this->command.channel.get()))
              .replace("#VIDEOLAYER#", QString("%1").arg(this->command.videolayer.get()));
    this->loopSubscription = new OscSubscription(loopFilter, this);
    QObject::connect(this->loopSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loopSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

    QString nextControlFilter = Osc::ITEM_CONTROL_NEXT_FILTER;
    nextControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->nextControlSubscription = new OscSubscription(nextControlFilter, this);
    QObject::connect(this->nextControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(nextControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString updateControlFilter = Osc::ITEM_CONTROL_UPDATE_FILTER;
    updateControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString previewControlFilter = Osc::ITEM_CONTROL_PREVIEW_FILTER;
    previewControlFilter.replace("#UID#", this->command.getRemoteTriggerId());
    this->previewControlSubscription = new OscSubscription(previewControlFilter, this);
    QObject::connect(this->previewControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(previewControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

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

void RundownMovieWidget::channelChanged(int channel)
{
    this->labelChannel->setText(QString("Channel: %1").arg(channel));

    configureOscSubscriptions();
}

void RundownMovieWidget::videolayerChanged(int videolayer)
{
    this->labelVideolayer->setText(QString("Video layer: %1").arg(videolayer));

    configureOscSubscriptions();
}

void RundownMovieWidget::delayChanged(int delay)
{
    this->labelDelay->setText(QString("Delay: %1").arg(delay));
}

void RundownMovieWidget::allowGpiChanged(bool allowGpi)
{
    Q_UNUSED(allowGpi);

    checkGpiConnection();
}

void RundownMovieWidget::loopChanged(bool loop)
{
    this->labelLoopOverlay->setVisible(loop);
}

void RundownMovieWidget::gpiConnectionStateChanged(bool connected, GpiDevice* device)
{
    Q_UNUSED(connected);
    Q_UNUSED(device);

    checkGpiConnection();
}

void RundownMovieWidget::remoteTriggerIdChanged(const QString& remoteTriggerId)
{
    configureOscSubscriptions();

    this->labelRemoteTriggerId->setText(QString("UID: %1").arg(remoteTriggerId));
}

void RundownMovieWidget::deviceConnectionStateChanged(CasparDevice& device)
{
    Q_UNUSED(device);

    checkDeviceConnection();
}

void RundownMovieWidget::deviceAdded(CasparDevice& device)
{
    if (DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress())->getName() == this->model.getDeviceName())
        QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));

    checkDeviceConnection();
    configureOscSubscriptions();
}

void RundownMovieWidget::frameSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    this->fileModel.setFrame(arguments.at(0).toInt());
    this->fileModel.setTotalFrames(arguments.at(1).toInt());

    updateOscWidget();
}

void RundownMovieWidget::fpsSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    this->fileModel.setFramesPerSecond(arguments.at(0).toDouble());

    updateOscWidget();
}

void RundownMovieWidget::pathSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    QString name = arguments.at(0).toString();
    name.remove(name.lastIndexOf('.'), name.length()); // Remove extension.

    if (this->model.getName().toLower() != name.toLower())
        return; // Wrong file.

    this->fileModel.setPath(arguments.at(0).toString());

    updateOscWidget();
}

void RundownMovieWidget::updateOscWidget()
{
    if (this->fileModel.getFrame() > 0 && this->fileModel.getTotalFrames() > 0 &&
        !this->fileModel.getPath().isEmpty() && this->fileModel.getFramesPerSecond() > 0)

    {
        this->widgetOscTime->setProgress(this->fileModel.getFrame());

        if (this->reverseOscTime && this->fileModel.getFrame() > 0)
            this->widgetOscTime->setTime(this->fileModel.getTotalFrames() - this->fileModel.getFrame());
        else
            this->widgetOscTime->setTime(this->fileModel.getFrame());

        if (this->command.getSeek() == 0 && this->command.getLength() == 0)
            this->widgetOscTime->setInOutTime(0, this->fileModel.getTotalFrames());
        else
            this->widgetOscTime->setInOutTime(this->command.getSeek(), this->command.getLength());

        this->widgetOscTime->setFramesPerSecond(this->fileModel.getFramesPerSecond());

        if (this->sendAutoPlay && !this->hasSentAutoPlay)
        {
            EventManager::getInstance().fireAutoPlayRundownItemEvent(AutoPlayRundownItemEvent(this));

            this->sendAutoPlay = false;
            this->hasSentAutoPlay = true;

            qDebug("Dispatched AutoPlay event");
        }

        this->playing = true;

        this->fileModel.setPath("");
        this->fileModel.setFrame(0);
        this->fileModel.setTotalFrames(0);
        this->fileModel.setFramesPerSecond(0);
    }
}

void RundownMovieWidget::pausedSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    this->widgetOscTime->setPaused(arguments.at(0).toBool());
}

void RundownMovieWidget::loopSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    this->widgetOscTime->setLoop(arguments.at(0).toBool());
}

void RundownMovieWidget::autoPlayChanged(bool autoPlay)
{
    this->labelAutoPlay->setVisible(autoPlay);
}

void RundownMovieWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Stop);
}

void RundownMovieWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Play);
}

void RundownMovieWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::PlayNow);
}

void RundownMovieWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Load);
}

void RundownMovieWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::PauseResume);
}

void RundownMovieWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Next);
}

void RundownMovieWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Update);
}

void RundownMovieWidget::previewControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Preview);
}

void RundownMovieWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::Clear);
}

void RundownMovieWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearVideoLayer);
}

void RundownMovieWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (this->command.getAllowRemoteTriggering() && arguments.count() > 0 && arguments[0].toInt() > 0)
        executeCommand(Playout::PlayoutType::ClearChannel);
}
