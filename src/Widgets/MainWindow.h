#pragma once

#include "Shared.h"
#include "ui_MainWindow.h"

#include "Events/ExportPresetMenuEvent.h"
#include "Events/SaveAsPresetMenuEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringEvent.h"
#include "Events/Rundown/RepositoryRundownEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/MarkItemAsUsedEvent.h"
#include "Events/Rundown/MarkItemAsUnusedEvent.h"
#include "Events/Rundown/MarkAllItemsAsUsedEvent.h"
#include "Events/Rundown/MarkAllItemsAsUnusedEvent.h"
#include "Events/Rundown/OpenRundownMenuEvent.h"
#include "Events/Rundown/ReloadRundownMenuEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>
#endif

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
        QMenu* markMenu;
        QMenu* playoutMenu;
        QMenu* helpMenu;
        QMenuBar* menuBar;

        QAction* newRundownAction;
        QAction* openRundownAction;
        QAction* openRundownFromUrlAction;
        QAction* exportPresetAction;
        QAction* saveAsPresetAction;
        QAction* saveAction;
        QAction* saveAsAction;
        QAction* allowRemoteTriggeringAction;
        QAction* insertRepositoryChangesAction;
        QAction* reloadRundownAction;

        void setupMenu();

        Q_SLOT void newRundown();
        Q_SLOT void openRundown();
        Q_SLOT void openRundownFromUrl();
        Q_SLOT void saveRundown();
        Q_SLOT void saveAsRundown();
        Q_SLOT void executeStop();
        Q_SLOT void executePlay();
        Q_SLOT void executePlayNow();
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
        Q_SLOT void reloadRundown();
        Q_SLOT void markItemAsUsed();
        Q_SLOT void markItemAsUnused();
        Q_SLOT void markAllItemsAsUsed();
        Q_SLOT void markAllItemsAsUnused();
        Q_SLOT void copyItemProperties();
        Q_SLOT void pasteItemProperties();
        Q_SLOT void insertRepositoryChanges();
        Q_SLOT void allowRemoteTriggering(bool);
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
        Q_SLOT void statusbar(const StatusbarEvent&);
        Q_SLOT void activeRundownChanged(const ActiveRundownChangedEvent&);
        Q_SLOT void newRundownMenu(const NewRundownMenuEvent&);
        Q_SLOT void openRundownMenu(const OpenRundownMenuEvent&);
        Q_SLOT void openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&);
        Q_SLOT void allowRemoteTriggering(const AllowRemoteTriggeringEvent&);
        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
        Q_SLOT void exportPresetMenu(const ExportPresetMenuEvent&);
        Q_SLOT void saveAsPresetMenu(const SaveAsPresetMenuEvent&);
        Q_SLOT void reloadRundownMenu(const ReloadRundownMenuEvent&);
};
