#pragma once

#include "Shared.h"
#include "ui_LiveWidget.h"
#include "LiveDialog.h"

#include "Events/CloseApplicationEvent.h"

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
#include <vlc/vlc.h>

#include <QtGui/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimediaWidgets/QVideoWidget>

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
        int streamPort;

        LiveDialog* liveDialog = nullptr;

        QMediaPlayer *mediaPlayer = nullptr;
        QAudioOutput *audioOutput = nullptr;
        QVideoWidget *videoWidget = nullptr;

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
        void setupRenderTarget(bool windowMode);
        void startStream();
        void stopStream();

        Q_SLOT void setupStreamMenu();
        Q_SLOT void setupAudioTrackMenu();
        Q_SLOT void toggleWindowMode();
        Q_SLOT void disconnectStream();
        Q_SLOT void muteAudio(bool);
        Q_SLOT void toggleExpandCollapse();
        Q_SLOT void streamMenuActionTriggered(QAction*);
        Q_SLOT void audioMenuActionTriggered(QAction*);
        Q_SLOT void closeApplication(const CloseApplicationEvent&);
};
