#include "LiveWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"
#include "Models/DeviceModel.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

#include <QtCore/QTimer>
#include <QtGui/QToolButton>

LiveWidget::LiveWidget(QWidget* parent)
    : QWidget(parent),
      collapsed(false), deviceName(""), deviceChannel("")
{
    setupUi(this);
    setupMenus();

    const char* const vlcArguments[] =
    {
        "-I", "dummy",
        "--ignore-config", // Don't use VLC's config.
        "--network-caching=200"
    };

    this->vlcInstance = libvlc_new(sizeof(vlcArguments) / sizeof(vlcArguments[0]), vlcArguments);
    this->vlcMediaPlayer = libvlc_media_player_new(this->vlcInstance);

    setupStream();
}

LiveWidget::~LiveWidget()
{
    stopStream(this->deviceName, this->deviceChannel);

    libvlc_media_player_stop(this->vlcMediaPlayer);
    libvlc_media_player_release(this->vlcMediaPlayer);
    libvlc_release(this->vlcInstance);
}

void LiveWidget::setupMenus()
{
    this->contextMenuLiveDropdown = new QMenu(this);
    this->contextMenuLiveDropdown->setTitle("Dropdown");

    this->streamMenu = new QMenu(this);
    this->streamMenu->setTitle("Connect to");
    this->streamMenuAction = this->contextMenuLiveDropdown->addMenu(this->streamMenu);
    this->contextMenuLiveDropdown->addSeparator();
    this->muteAction = this->contextMenuLiveDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Mute Audio");
    this->muteAction->setCheckable(true);
    this->contextMenuLiveDropdown->addSeparator();
    this->contextMenuLiveDropdown->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Expand / Collapse", this, SLOT(toggleExpandCollapse()));

    QObject::connect(this->streamMenuAction, SIGNAL(hovered()), this, SLOT(streamMenuHovered()));
    QObject::connect(this->streamMenu, SIGNAL(triggered(QAction*)), this, SLOT(streamMenuActionTriggered(QAction*)));
    QObject::connect(this->muteAction, SIGNAL(toggled(bool)), this, SLOT(muteAudio(bool)));

    QToolButton* toolButtonLiveDropdown = new QToolButton(this);
    toolButtonLiveDropdown->setObjectName("toolButtonLiveDropdown");
    toolButtonLiveDropdown->setMenu(this->contextMenuLiveDropdown);
    toolButtonLiveDropdown->setPopupMode(QToolButton::InstantPopup);

    this->tabWidgetLive->setCornerWidget(toolButtonLiveDropdown);
    //this->tabWidgetPreview->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));
}

void LiveWidget::setupStreamMenu()
{
    foreach (QAction* action, this->streamMenu->actions())
        this->streamMenu->removeAction(action);

    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();
    foreach (DeviceModel model, models)
    {
        for (int i = 0; i < model.getChannels(); i++)
            this->streamMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ QString("%1:%2").arg(model.getName()).arg(i + 1));
    }

    this->streamMenu->addSeparator();
    this->streamMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Disconnect", this, SLOT(disconnectStream()));
}

void LiveWidget::streamMenuHovered()
{
    setupStreamMenu();
}

void LiveWidget::streamMenuActionTriggered(QAction* action)
{
    if (!action->text().contains(':'))
        return;

    stopStream(this->deviceName, this->deviceChannel);

    this->deviceName = action->text().split(':')[0];
    this->deviceChannel = action->text().split(':')[1];

    startStream(this->deviceName, this->deviceChannel);
}

void LiveWidget::setupStream()
{
    this->vlcMedia = libvlc_media_new_location(this->vlcInstance, "udp://@0.0.0.0:5004");
    libvlc_media_player_set_media(this->vlcMediaPlayer, this->vlcMedia);

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(this->vlcMediaPlayer, this->labelLive->winId());
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_agl(this->vlcMediaPlayer, this->labelLive->winId());
#elif defined(Q_OS_LINUX)
    libvlc_media_player_set_xwindow(this->vlcMediaPlayer, this->labelLive->winId());
#endif
}

void LiveWidget::disconnectStream()
{
    stopStream(this->deviceName, this->deviceChannel);
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
            device->startStream(this->deviceChannel.toInt(), 5004);
    }
}

void LiveWidget::muteAudio(bool mute)
{
    libvlc_audio_set_mute(this->vlcMediaPlayer, mute);
}

void LiveWidget::toggleExpandCollapse()
{
    this->collapsed = !this->collapsed;

    this->setFixedHeight((this->collapsed == true) ? Panel::COMPACT_LIVE_HEIGHT : Panel::DEFAULT_LIVE_HEIGHT);
}
