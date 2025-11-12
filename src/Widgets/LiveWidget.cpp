#include "LiveWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"
#include "Models/DeviceModel.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtWidgets/QToolButton>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimediaWidgets/QVideoWidget>

LiveWidget::LiveWidget(QWidget *parent)
    : QWidget(parent),
      collapsed(false), windowMode(false), deviceName(""), deviceChannel(""), useKey(false), mediaPlayer(NULL), audioOutput(NULL), videoWidget(NULL)
{
    setupUi(this);
    setupMenus();

    this->liveDialog = new LiveDialog(this);
    QObject::connect(this->liveDialog, SIGNAL(rejected()), this, SLOT(toggleWindowMode()));

    QString streamPort = DatabaseManager::getInstance().getConfigurationByName("StreamPort").getValue();
    this->streamPort = (streamPort.isEmpty() == true) ? Stream::DEFAULT_PORT : streamPort.toInt();

    QObject::connect(&EventManager::getInstance(), SIGNAL(closeApplication(const CloseApplicationEvent &)), this, SLOT(closeApplication(const CloseApplicationEvent &)));
}

void LiveWidget::closeApplication(const CloseApplicationEvent &event)
{
    Q_UNUSED(event);

    stopStream();

    if (this->mediaPlayer != nullptr)
    {
        this->mediaPlayer->stop();
        delete this->mediaPlayer;
        this->mediaPlayer = nullptr;
    }

    if (this->audioOutput != nullptr)
    {
        delete this->audioOutput;
        this->audioOutput = nullptr;
    }

    if (this->videoWidget != nullptr)
    {
        delete this->videoWidget;
        this->videoWidget = nullptr;
    }
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

    QObject::connect(this->streamMenu, SIGNAL(aboutToShow()), this, SLOT(setupStreamMenu()));
    QObject::connect(this->streamMenu, SIGNAL(triggered(QAction *)), this, SLOT(streamMenuActionTriggered(QAction *)));
    QObject::connect(this->audioTrackMenu, SIGNAL(aboutToShow()), this, SLOT(setupAudioTrackMenu()));
    QObject::connect(this->audioTrackMenu, SIGNAL(triggered(QAction *)), this, SLOT(audioMenuActionTriggered(QAction *)));
    QObject::connect(this->muteAction, SIGNAL(toggled(bool)), this, SLOT(muteAudio(bool)));

    QToolButton *toolButtonLiveDropdown = new QToolButton(this);
    toolButtonLiveDropdown->setObjectName("toolButtonLiveDropdown");
    toolButtonLiveDropdown->setMenu(this->contextMenuLiveDropdown);
    toolButtonLiveDropdown->setPopupMode(QToolButton::InstantPopup);

    this->tabWidgetLive->setCornerWidget(toolButtonLiveDropdown);
    // this->tabWidgetPreview->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));
}

void LiveWidget::setupAudioTrackMenu()
{
    if (!this->mediaPlayer)
        return;

    if (this->mediaPlayer->playbackState() != QMediaPlayer::PlayingState)
        return;

    foreach (QAction *action, this->audioTrackMenu->actions())
        this->audioTrackMenu->removeAction(action);

    // QMediaPlayer handles audio tracks automatically in Qt6
    // For basic usage, we'll keep the menu but add a default option
    this->audioTrackAction = this->audioTrackMenu->addAction("Default Audio Track");
    this->audioTrackAction->setCheckable(true);
    this->audioTrackAction->setChecked(true);
    this->audioTrackAction->setData(0);
}

void LiveWidget::setupStreamMenu()
{
    foreach (QAction *action, this->streamMenu->actions())
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

void LiveWidget::audioMenuActionTriggered(QAction *action)
{
    if (!this->mediaPlayer)
        return;

    // QMediaPlayer handles audio tracks automatically
    // Additional implementation can be added here if needed
}

void LiveWidget::streamMenuActionTriggered(QAction *action)
{
    if (!action->text().contains(',') && !action->text().contains(':'))
        return;

    stopStream();

    this->useKey = action->text().contains("(Key)");
    this->deviceName = action->text().split(',').at(0).split(':').at(1).trimmed();
    this->deviceChannel = action->text().split(',').at(1).split(':').at(1).trimmed().split(' ').at(0).trimmed();

    startStream();
}

void LiveWidget::disconnectStream()
{
    stopStream();

    this->deviceName.clear();
    this->deviceChannel.clear();
}

void LiveWidget::startStream()
{
    if (!this->deviceName.isEmpty() && !this->deviceChannel.isEmpty())
    {
        bool disableAudioInStream = (DatabaseManager::getInstance().getConfigurationByName("DisableAudioInStream").getValue() == "true") ? true : false;

        qDebug("Starting media player for stream on port: %d", this->streamPort);

        // Create media player if it doesn't exist
        if (!this->mediaPlayer)
        {
            this->mediaPlayer = new QMediaPlayer(this);
            this->audioOutput = new QAudioOutput(this);
            this->mediaPlayer->setAudioOutput(this->audioOutput);
            
            // Connect error handling
            QObject::connect(this->mediaPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
                qWarning("Media player error %d: %s", error, qPrintable(errorString));
            });
        }

        // Set up video output
        setupRenderTarget(this->windowMode);

        // Set the stream source with larger buffers and error tolerance
        QString streamUrl = QString("udp://0.0.0.0:%1?overrun_nonfatal=1&fifo_size=100000000&buffer_size=10000000").arg(this->streamPort);
        qDebug("Setting media source: %s", qPrintable(streamUrl));
        this->mediaPlayer->setSource(QUrl(streamUrl));

        // Configure audio
        if (disableAudioInStream)
        {
            this->audioOutput->setMuted(true);
        }

        // Start playback
        this->mediaPlayer->play();

        // Start the stream on the CasparCG device
        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->deviceName);
        if (device != NULL && device->isConnected())
        {
            int quality = DatabaseManager::getInstance().getConfigurationByName("StreamQuality").getValue().toInt();

            if (this->windowMode)
                device->startStream(this->deviceChannel.toInt(), this->streamPort, quality, this->useKey);
            else
                device->startStream(this->deviceChannel.toInt(), this->streamPort, quality, this->useKey, Stream::COMPACT_WIDTH, Stream::COMPACT_HEIGHT);
        }
    }
}

void LiveWidget::setupRenderTarget(bool windowMode)
{
    if (!this->mediaPlayer)
        return;

    // Clean up existing video widget if it exists
    if (this->videoWidget)
    {
        this->mediaPlayer->setVideoOutput((QVideoWidget *)nullptr);
        delete this->videoWidget;
        this->videoWidget = nullptr;
    }

    // Create a new video widget for the appropriate target
    QWidget *targetWidget = (windowMode == true) ? this->liveDialog->getRenderTarget() : this->labelLiveSmall;

    this->videoWidget = new QVideoWidget(targetWidget);
    this->videoWidget->setGeometry(targetWidget->rect());
    this->videoWidget->show();

    // Set the video output
    this->mediaPlayer->setVideoOutput(this->videoWidget);
}

void LiveWidget::stopStream()
{
    if (!this->mediaPlayer)
        return;

    if (this->deviceName.isEmpty() || this->deviceChannel.isEmpty())
        return;

    this->mediaPlayer->stop();

     // Clean up existing video widget if it exists
    if (this->videoWidget)
    {
        this->mediaPlayer->setVideoOutput((QVideoWidget *)nullptr);
        delete this->videoWidget;
        this->videoWidget = nullptr;
    }

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(this->deviceName);
    if (device != NULL && device->isConnected())
        device->stopStream(this->deviceChannel.toInt(), this->streamPort);
}

void LiveWidget::muteAudio(bool mute)
{
    if (!this->audioOutput)
        return;

    this->audioOutput->setMuted(mute);
}

void LiveWidget::toggleExpandCollapse()
{
    this->collapsed = !this->collapsed;

    this->expandCollapseAction->setText((this->collapsed == true) ? "Expand" : "Collapse");
    this->setFixedHeight((this->collapsed == true) ? Panel::COMPACT_LIVE_HEIGHT : Panel::DEFAULT_LIVE_HEIGHT);
}

void LiveWidget::toggleWindowMode()
{
    stopStream();

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
    startStream();
}
