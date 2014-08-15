#pragma once

#include "Shared.h"
#include "ui_LiveWidget.h"
#include "LiveDialog.h"

#include "Events/CloseApplicationEvent.h"

#include <vlc/vlc.h>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class WIDGETS_EXPORT LiveWidget : public QWidget, Ui::LiveWidget
{
    Q_OBJECT

    public:
        explicit LiveWidget(QWidget* parent = 0);

    private:
        bool collapsed;
        bool windowMode;
        QString deviceName;
        QString deviceChannel;
        bool useKey;

        LiveDialog* liveDialog;

        libvlc_media_t* vlcMedia;
        libvlc_instance_t* vlcInstance;
        libvlc_media_player_t* vlcMediaPlayer;

        QMenu* streamMenu;
        QMenu* audioMenu;
        QMenu* audioTrackMenu;
        QMenu* contextMenuLiveDropdown;

        QAction* muteAction;
        QAction* windowModeAction;
        QAction* streamMenuAction;
        QAction* audioTrackAction;
        QAction* audioTrackMenuAction;
        QAction* expandCollapseAction;

        void setupMenus();
        void setupAudioTrackMenu();
        void setupStreamMenu();
        void setupRenderTarget(bool windowMode);
        void startStream(const QString& deviceName, const QString& deviceChannel);
        void stopStream(const QString& deviceName, const QString& deviceChannel);

        Q_SLOT void toggleWindowMode();
        Q_SLOT void disconnectStream();
        Q_SLOT void muteAudio(bool);
        Q_SLOT void toggleExpandCollapse();
        Q_SLOT void audioTrackMenuHovered();
        Q_SLOT void streamMenuHovered();
        Q_SLOT void streamMenuActionTriggered(QAction*);
        Q_SLOT void audioMenuActionTriggered(QAction*);
        Q_SLOT void closeApplication(const CloseApplicationEvent&);
};
