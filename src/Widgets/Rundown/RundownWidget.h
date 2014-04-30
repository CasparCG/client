#pragma once

#include "../Shared.h"
#include "ui_RundownWidget.h"

#include "Global.h"

#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/CloseRundownEvent.h"
#include "Events/Rundown/DeleteRundownEvent.h"
#include "Events/Rundown/NewRundownEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/OpenRundownFromUrlEvent.h"
#include "Events/Rundown/OpenRundownMenuEvent.h"
#include "Events/Rundown/OpenRundownFromUrlMenuEvent.h"
#include "Events/Rundown/ReloadRundownEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QKeyEvent>
#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownWidget : public QWidget, Ui::RundownWidget
{
    Q_OBJECT

    public:
        explicit RundownWidget(QWidget* parent = 0);

    private:
        QMenu* contextMenuRundownDropdown;

        QAction* newRundownAction;
        QAction* openRundownAction;
        QAction* openRundownFromUrlAction;
        QAction* allowRemoteTriggeringAction;

        void setupMenus();

        Q_SLOT void openRundownFromDisk();
        Q_SLOT void openRundownFromRepo();
        Q_SLOT void reloadCurrentRundown();
        Q_SLOT void closeCurrentRundown();
        Q_SLOT void createNewRundown();
        Q_SLOT void saveRundownToDisk();
        Q_SLOT void saveAsRundownToDisk();
        Q_SLOT void toggleCompactView();
        Q_SLOT bool selectTab(int index);
        Q_SLOT void currentChanged(int);
        Q_SLOT void tabCloseRequested(int);
        Q_SLOT void gpiBindingChanged(int, Playout::PlayoutType::Type);
        Q_SLOT void allowRemoteTriggering(bool enabled);
        Q_SLOT void newRundownMenu(const NewRundownMenuEvent&);
        Q_SLOT void openRundownMenu(const OpenRundownMenuEvent&);
        Q_SLOT void openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&);
        Q_SLOT void newRundown(const NewRundownEvent&);
        Q_SLOT void allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent&);
        Q_SLOT void closeRundown(const CloseRundownEvent&);
        Q_SLOT void deleteRundown(const DeleteRundownEvent&);
        Q_SLOT void openRundown(const OpenRundownEvent&);
        Q_SLOT void openRundownFromUrl(const OpenRundownFromUrlEvent&);
        Q_SLOT void saveRundown(const SaveRundownEvent&);
        Q_SLOT void activeRundownChanged(const ActiveRundownChangedEvent&);
        Q_SLOT void reloadRundown(const ReloadRundownEvent&);
};
