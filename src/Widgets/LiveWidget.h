#pragma once

#include "Shared.h"
#include "ui_LiveWidget.h"

#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <vlc/vlc.h>

#include <QtGui/QAction>
#include <QtGui/QImage>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class WIDGETS_EXPORT LiveWidget : public QWidget, Ui::LiveWidget
{
    Q_OBJECT

    public:
        explicit LiveWidget(QWidget* parent = 0);
        ~LiveWidget();

    private:
        QImage image;
        bool collapsed;
        LibraryModel* model;
        QString deviceName;
        QString deviceChannel;

        libvlc_media_t* vlcMedia;
        libvlc_instance_t* vlcInstance;
        libvlc_media_player_t* vlcMediaPlayer;

        QMenu* streamMenu;
        QMenu* contextMenuLiveDropdown;

        QAction* muteAction;
        QAction* streamMenuAction;

        void setupMenus();
        void setupStreamMenu();
        void setupStream();
        void startStream(const QString& deviceName, const QString& deviceChannel);
        void stopStream(const QString& deviceName, const QString& deviceChannel);

        Q_SLOT void disconnectStream();
        Q_SLOT void muteAudio(bool);
        Q_SLOT void toggleExpandCollapse();
        Q_SLOT void streamMenuHovered();
        Q_SLOT void streamMenuActionTriggered(QAction*);
};
