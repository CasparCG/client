#include "LiveWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"
#include "Models/DeviceModel.h"

#include <QtGui/QToolButton>

LiveWidget::LiveWidget(QWidget* parent)
    : QWidget(parent),
      collapsed(false), windowMode(false), deviceName(""), deviceChannel(""), useKey(false), vlcMedia(NULL), vlcInstance(NULL), vlcMediaPlayer(NULL)
{
    setupUi(this);
    setupMenus();

    this->liveDialog = new LiveDialog(this);
    QObject::connect(this->liveDialog, SIGNAL(rejected()), this, SLOT(toggleWindowMode()));

    QStringList arguments;
    arguments.append("--no-osd");
    arguments.append("--ignore-config");
    arguments.append(QString("--network-caching=%1").arg(DatabaseManager::getInstance().getConfigurationByName("NetworkCache").getValue().toInt()));

    bool disableAudioInStream = (DatabaseManager::getInstance().getConfigurationByName("DisableAudioInStream").getValue() == "true") ? true : false;
    if (disableAudioInStream)
        arguments.append("--no-audio");

    QString args;
    foreach (QString value, arguments)
        args += value + " ";

    qDebug() << QString("LiveWidget::LiveWidget: Using arguments: %1").arg(args.trimmed());

    char* vlcArguments[arguments.count()];
    for (int i = 0; i < arguments.count(); i++)
        vlcArguments[i] = (char*)qstrdup(arguments.at(i).toUtf8().data());

    this->vlcInstance = libvlc_new(sizeof(vlcArguments) / sizeof(vlcArguments[0]), vlcArguments);
    this->vlcMediaPlayer = libvlc_media_player_new(this->vlcInstance);

    this->vlcMedia = libvlc_media_new_location(this->vlcInstance, "udp://@0.0.0.0:5004");
    libvlc_media_player_set_media(this->vlcMediaPlayer, this->vlcMedia);

    setupRenderTarget(this->windowMode);

    QObject::connect(&EventManager::getInstance(), SIGNAL(closeApplication(const CloseApplicationEvent&)), this, SLOT(closeApplication(const CloseApplicationEvent&)));
}

void LiveWidget::closeApplication(const CloseApplicationEvent& event)
{
    stopStream(this->deviceName, this->deviceChannel);

    libvlc_media_player_release(this->vlcMediaPlayer);
    libvlc_release(this->vlcInstance);

    this->vlcMediaPlayer = NULL;
    this->vlcInstance = NULL;
}

void LiveWidget::setupMenus()
{
    this->contextMenuLiveDropdown = new QMenu(this);
    this->contextMenuLiveDropdown->setTitle("Dropdown");

    this->audioTrackMenu = new QMenu(this);
    this->audioTrackMenu->setTitle("Audio Track");

    this->audioMenu = new QMenu(this);
    this->audioMenu->setTitle("Audio");
    this->audioTrackMenuAction = this->audioMenu->addMenu(this->audioTrackMenu);
    this->audioMenu->addSeparator();
    this->muteAction = this->audioMenu->addAction(/*QIcon(":/Graphics/Images/MuteSound.png"),*/ "Mute");
    this->muteAction->setCheckable(true);

    this->streamMenu = new QMenu(this);
    this->streamMenu->setTitle("Connect to");
    this->streamMenuAction = this->contextMenuLiveDropdown->addMenu(this->streamMenu);
    this->contextMenuLiveDropdown->addSeparator();
    this->contextMenuLiveDropdown->addMenu(this->audioMenu);
    this->contextMenuLiveDropdown->addSeparator();
    this->windowModeAction = this->contextMenuLiveDropdown->addAction(/*QIcon(":/Graphics/Images/WindowMode.png"),*/ "Window Mode", this, SLOT(toggleWindowMode()));
    this->windowModeAction->setCheckable(true);
    this->contextMenuLiveDropdown->addSeparator();
    this->expandCollapseAction = this->contextMenuLiveDropdown->addAction(/*QIcon(":/Graphics/Images/Collapse.png"),*/ "Collapse", this, SLOT(toggleExpandCollapse()));

    QObject::connect(this->streamMenuAction, SIGNAL(hovered()), this, SLOT(streamMenuHovered()));
    QObject::connect(this->audioTrackMenuAction, SIGNAL(hovered()), this, SLOT(audioTrackMenuHovered()));
    QObject::connect(this->streamMenu, SIGNAL(triggered(QAction*)), this, SLOT(streamMenuActionTriggered(QAction*)));
    QObject::connect(this->audioTrackMenu, SIGNAL(triggered(QAction*)), this, SLOT(audioMenuActionTriggered(QAction*)));
    QObject::connect(this->muteAction, SIGNAL(toggled(bool)), this, SLOT(muteAudio(bool)));

    QToolButton* toolButtonLiveDropdown = new QToolButton(this);
    toolButtonLiveDropdown->setObjectName("toolButtonLiveDropdown");
    toolButtonLiveDropdown->setMenu(this->contextMenuLiveDropdown);
    toolButtonLiveDropdown->setPopupMode(QToolButton::InstantPopup);

    this->tabWidgetLive->setCornerWidget(toolButtonLiveDropdown);
    //this->tabWidgetPreview->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));
}

void LiveWidget::setupRenderTarget(bool windowMode)
{
#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(this->vlcMediaPlayer, (windowMode == true) ? this->liveDialog->getRenderTarget()->winId() : this->labelLive->winId());
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(this->vlcMediaPlayer, (windowMode == true) ? (void*)this->liveDialog->getRenderTarget()->winId() : (void*)this->labelLive->winId());
#elif defined(Q_OS_LINUX)
    libvlc_media_player_set_xwindow(this->vlcMediaPlayer, (windowMode == true) ? this->liveDialog->getRenderTarget()->winId() : this->labelLive->winId());
#endif
}

void LiveWidget::audioTrackMenuHovered()
{
    setupAudioTrackMenu();
}

void LiveWidget::setupAudioTrackMenu()
{
    if (!libvlc_media_player_is_playing(this->vlcMediaPlayer))
        return;

    foreach (QAction* action, this->audioTrackMenu->actions())
        this->audioTrackMenu->removeAction(action);

    int currentTrack = libvlc_audio_get_track(this->vlcMediaPlayer);
    libvlc_track_description_t* trackDescription = libvlc_audio_get_track_description(this->vlcMediaPlayer);
    while (trackDescription != NULL)
    {
        this->audioTrackAction = this->audioTrackMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ trackDescription->psz_name);
        this->audioTrackAction->setCheckable(true);
        this->audioTrackAction->setData(trackDescription->i_id);

        if (trackDescription->i_id == currentTrack)
            this->audioTrackAction->setChecked(true);

        trackDescription = trackDescription->p_next;
    }
}

void LiveWidget::audioMenuActionTriggered(QAction* action)
{
    libvlc_audio_set_track(this->vlcMediaPlayer, action->data().toInt());
}

void LiveWidget::streamMenuHovered()
{
    setupStreamMenu();
}

void LiveWidget::setupStreamMenu()
{
    foreach (QAction* action, this->streamMenu->actions())
        this->streamMenu->removeAction(action);

    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();
    foreach (DeviceModel model, models)
    {
        for (int i = 0; i < model.getChannels(); i++)
        {
            this->streamMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ QString("Device: %1, Channel: %2 (Fill)").arg(model.getName()).arg(i + 1));
            this->streamMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ QString("Device: %1, Channel: %2 (Key)").arg(model.getName()).arg(i + 1));
        }
    }

    this->streamMenu->addSeparator();
    this->streamMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Disconnect", this, SLOT(disconnectStream()));
}

void LiveWidget::streamMenuActionTriggered(QAction* action)
{
    if (!action->text().contains(',') && !action->text().contains(':'))
        return;

    stopStream(this->deviceName, this->deviceChannel);

    this->useKey = action->text().contains("(Key)");
    this->deviceName = action->text().split(',').at(0).split(':').at(1).trimmed();
    this->deviceChannel = action->text().split(',').at(1).split(':').at(1).trimmed().split(' ').at(0).trimmed();

    startStream(this->deviceName, this->deviceChannel);
}

void LiveWidget::disconnectStream()
{
    stopStream(this->deviceName, this->deviceChannel);

    this->deviceName.clear();
    this->deviceChannel.clear();
}

void LiveWidget::stopStream(const QString& deviceName, const QString& deviceChannel)
{
    if (!this->deviceName.isEmpty() && !this->deviceChannel.isEmpty())
    {
        libvlc_media_player_stop(this->vlcMediaPlayer);

        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->deviceName);
        if (device != NULL && device->isConnected())
            device->stopStream(this->deviceChannel.toInt(), 5004);
    }
}

void LiveWidget::startStream(const QString& deviceName, const QString& deviceChannel)
{
    if (!this->deviceName.isEmpty() && !this->deviceChannel.isEmpty())
    {
        libvlc_media_player_play(this->vlcMediaPlayer);

        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->deviceName);
        if (device != NULL && device->isConnected())
        {
            int quality = DatabaseManager::getInstance().getConfigurationByName("StreamQuality").getValue().toInt();

            if (this->windowMode)
                device->startStream(this->deviceChannel.toInt(), 5004, quality, this->useKey);
            else
                device->startStream(this->deviceChannel.toInt(), 5004, quality, this->useKey, Stream::COMPACT_WIDTH, Stream::COMPACT_HEIGHT);
        }
    }
}

void LiveWidget::muteAudio(bool mute)
{
    libvlc_audio_set_mute(this->vlcMediaPlayer, mute);
}

void LiveWidget::toggleExpandCollapse()
{
    this->collapsed = !this->collapsed;

    this->expandCollapseAction->setText((this->collapsed == true) ? "Expand" : "Collapse");
    this->setFixedHeight((this->collapsed == true) ? Panel::COMPACT_LIVE_HEIGHT : Panel::DEFAULT_LIVE_HEIGHT);
}

void LiveWidget::toggleWindowMode()
{
    stopStream(this->deviceName, this->deviceChannel);

    if (this->windowMode)
    {
        this->windowModeAction->blockSignals(true);
        this->windowModeAction->setChecked(false);
        this->windowModeAction->blockSignals(false);

        this->liveDialog->hide();
    }
    else
    {
        this->liveDialog->visible();
    }

    this->windowMode = !this->windowMode;
    setupRenderTarget(this->windowMode);
    startStream(this->deviceName, this->deviceChannel);
}
