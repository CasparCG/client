#pragma once

#include "Shared.h"
#include "ui_MainWindow.h"

#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/OpenRundownMenuEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

class WIDGETS_EXPORT MainWindow : public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);

    protected:
        virtual void closeEvent(QCloseEvent* event);

    private:
        QString applicationTitle;

        QMenu* fileMenu;
        QMenu* editMenu;
        QMenu* viewMenu;
        QMenu* libraryMenu;
        QMenu* rundownMenu;
        QMenu* playoutMenu;
        QMenu* helpMenu;
        QMenuBar* menuBar;

        QAction* newRundownAction;
        QAction* openRundownAction;
        QAction* openRundownFromUrlAction;
        //QAction* exportPresetAction;
        QAction* saveAsPresetAction;
        QAction* allowRemoteTriggeringAction;

        void setupMenu();

        Q_SLOT void newRundown();
        Q_SLOT void openRundown();
        Q_SLOT void openRundownFromUrl();
        Q_SLOT void saveRundown();
        Q_SLOT void saveAsRundown();
        Q_SLOT void executeStop();
        Q_SLOT void executePlay();
        Q_SLOT void executePause();
        Q_SLOT void executeLoad();
        Q_SLOT void executeNext();
        Q_SLOT void executeUpdate();
        Q_SLOT void executeInvoke();
        Q_SLOT void executePreview();
        Q_SLOT void executeClear();
        Q_SLOT void executeClearVideolayer();
        Q_SLOT void executeClearChannel();
        Q_SLOT void showAboutDialog();
        Q_SLOT void showHelpDialog();
        Q_SLOT void showSettingsDialog();
        Q_SLOT void toggleFullscreen();
        Q_SLOT void toggleCompactView();
        Q_SLOT void refreshLibrary();
        Q_SLOT void importPreset();
        Q_SLOT void exportPreset();
        Q_SLOT void saveAsPreset();
        Q_SLOT void closeRundown();
        Q_SLOT void allowRemoteTriggering(bool);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
        Q_SLOT void statusbar(const StatusbarEvent&);
        Q_SLOT void activeRundownChanged(const ActiveRundownChangedEvent&);
        Q_SLOT void newRundownMenu(const NewRundownMenuEvent&);
        Q_SLOT void openRundownMenu(const OpenRundownMenuEvent&);
        Q_SLOT void openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&);
        Q_SLOT void allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent&);
};
