#include "MainWindow.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include "Version.h"
#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"
#include "Events/ExportPresetEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/SaveAsPresetEvent.h"
#include "Events/Rundown/CloseRundownEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/NewRundownEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Rundown/ExecutePlayoutCommandEvent.h"
#include "Events/Rundown/RemoteRundownTriggeringEvent.h"

#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolButton>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    setupMenu();
    setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));

    setWindowTitle(QString("%1 %2.%3.%4").arg(this->windowTitle()).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION));
    this->applicationTitle = this->windowTitle();

    this->widgetAction->setVisible(false);

    this->splitterHorizontal->setSizes(QList<int>() << 1 << 0);
    this->splitterVertical->setSizes(QList<int>() << 289 << 860 << 289);

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(statusbar(const StatusbarEvent&)), this, SLOT(statusbar(const StatusbarEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(emptyRundown(const EmptyRundownEvent&)), this, SLOT(emptyRundown(const EmptyRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(activeRundownChanged(const ActiveRundownChangedEvent&)), this, SLOT(activeRundownChanged(const ActiveRundownChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(newRundownMenu(const NewRundownMenuEvent&)), this, SLOT(newRundownMenu(const NewRundownMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundownMenu(const OpenRundownMenuEvent&)), this, SLOT(openRundownMenu(const OpenRundownMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&)), this, SLOT(openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent&)), this, SLOT(allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent&)));
}

void MainWindow::setupMenu()
{
    this->fileMenu = new QMenu(this);
    this->newRundownAction = this->fileMenu->addAction("New Rundown", this, SLOT(newRundown()));
    this->openRundownAction = this->fileMenu->addAction("Open Rundown...", this, SLOT(openRundown()));
    this->openRundownFromUrlAction = this->fileMenu->addAction("Open Rundown from repository...", this, SLOT(openRundownFromUrl()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Import Preset...", this, SLOT(importPreset()));
    this->fileMenu->addAction("Export Preset...", this, SLOT(exportPreset()));
    this->saveAsPresetAction = this->fileMenu->addAction("Save as Preset...", this, SLOT(saveAsPreset()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Save", this, SLOT(saveRundown()), QKeySequence::fromString("Ctrl+S"));
    this->fileMenu->addAction("Save As...", this, SLOT(saveAsRundown()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Quit", this, SLOT(close()));
    //this->exportPresetAction = this->fileMenu->actions().at(4)->setEnabled(false);
    this->saveAsPresetAction->setEnabled(false);

    this->editMenu = new QMenu(this);
    this->editMenu->addAction("Settings...", this, SLOT(showSettingsDialog()));

    this->viewMenu = new QMenu(this);
    this->viewMenu->addAction("Toggle Fullscreen", this, SLOT(toggleFullscreen()), QKeySequence::fromString("Ctrl+F"));

    this->libraryMenu = new QMenu(this);
    this->libraryMenu->addAction("Refresh Library", this, SLOT(refreshLibrary()), QKeySequence::fromString("Ctrl+R"));

    this->rundownMenu = new QMenu(this);
    this->rundownMenu->addAction("Toggle Compact View", this, SLOT(toggleCompactView()));
    this->allowRemoteTriggeringAction = this->rundownMenu->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "Allow Remote Triggering");
    this->rundownMenu->addSeparator();
    this->rundownMenu->addAction("Close Rundown", this, SLOT(closeRundown()), QKeySequence::fromString("Ctrl+W"));
    this->allowRemoteTriggeringAction->setCheckable(true);
    QObject::connect(this->allowRemoteTriggeringAction, SIGNAL(toggled(bool)), this, SLOT(allowRemoteTriggering(bool)));

    this->playoutMenu = new QMenu(this);
    this->playoutMenu->addAction("Stop", this, SLOT(executeStop()), QKeySequence::fromString("F1"));
    this->playoutMenu->addAction("Play", this, SLOT(executePlay()), QKeySequence::fromString("F2"));
    this->playoutMenu->addAction("Load", this, SLOT(executeLoad()), QKeySequence::fromString("F3"));
    this->playoutMenu->addAction("Pause", this, SLOT(executePause()), QKeySequence::fromString("F4"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Next", this, SLOT(executeNext()), QKeySequence::fromString("F5"));
    this->playoutMenu->addAction("Update", this, SLOT(executeUpdate()), QKeySequence::fromString("F6"));
    this->playoutMenu->addAction("Invoke", this, SLOT(executeInvoke()), QKeySequence::fromString("F7"));
    this->playoutMenu->addAction("Preview", this, SLOT(executePreview()), QKeySequence::fromString("F8"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Clear", this, SLOT(executeClear()), QKeySequence::fromString("F10"));
    this->playoutMenu->addAction("Clear Video Layer", this, SLOT(executeClearVideolayer()), QKeySequence::fromString("F11"));
    this->playoutMenu->addAction("Clear Channel", this, SLOT(executeClearChannel()), QKeySequence::fromString("F12"));

    this->helpMenu = new QMenu(this);
    QAction* action = this->helpMenu->addAction("View Help", this, SLOT(showHelpDialog()), QKeySequence::fromString("Ctrl+H"));
    this->helpMenu->addSeparator();
    this->helpMenu->addAction("About CasparCG Client...", this, SLOT(showAboutDialog()));
    action->setEnabled(false);

    this->menuBar = new QMenuBar(this);
    this->menuBar->addMenu(this->fileMenu)->setText("File");
    this->menuBar->addMenu(this->editMenu)->setText("Edit");
    this->menuBar->addMenu(this->viewMenu)->setText("View");
    this->menuBar->addMenu(this->libraryMenu)->setText("Library");
    this->menuBar->addMenu(this->rundownMenu)->setText("Rundown");
    this->menuBar->addMenu(this->playoutMenu)->setText("Playout");
    this->menuBar->addMenu(this->helpMenu)->setText("Help");

    setMenuBar(this->menuBar);
}

void MainWindow::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->saveAsPresetAction->setEnabled(true);
}

void MainWindow::emptyRundown(const EmptyRundownEvent& event)
{
    this->saveAsPresetAction->setEnabled(false);
}

void MainWindow::statusbar(const StatusbarEvent& event)
{
    this->statusBar()->showMessage(event.getMessage(), event.getTimeout());
}

void MainWindow::activeRundownChanged(const ActiveRundownChangedEvent& event)
{
    QFileInfo info(event.getPath());
    if (info.baseName() == Rundown::DEFAULT_NAME)
        this->setWindowTitle(QString("%1").arg(this->applicationTitle));
    else
        this->setWindowTitle(QString("%1 - %2").arg(this->applicationTitle).arg(event.getPath()));
}

void MainWindow::newRundownMenu(const NewRundownMenuEvent& event)
{
    this->newRundownAction->setEnabled(event.getEnabled());
}

void MainWindow::openRundownMenu(const OpenRundownMenuEvent& event)
{
    this->openRundownAction->setEnabled(event.getEnabled());
}

void MainWindow::openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent& event)
{
    this->openRundownFromUrlAction->setEnabled(event.getEnabled());
}

void MainWindow::allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent& event)
{
    // We do not want to trigger check changed event.
    this->allowRemoteTriggeringAction->blockSignals(true);
    this->allowRemoteTriggeringAction->setChecked(event.getEnabled());
    this->allowRemoteTriggeringAction->blockSignals(false);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox box(this);
    box.setWindowTitle("Quit");
    box.setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));
    box.setText("Are you sure you want to quit? Unsaved changes to your rundowns will be lost!");
    box.setIconPixmap(QPixmap(":/Graphics/Images/Attention.png"));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.buttons().at(0)->setIcon(QIcon());
    box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
    box.buttons().at(1)->setIcon(QIcon());
    box.buttons().at(1)->setFocusPolicy(Qt::NoFocus);

    if (box.exec() == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::importPreset()
{
    EventManager::getInstance().fireImportPresetEvent(ImportPresetEvent());
}

void MainWindow::exportPreset()
{
    EventManager::getInstance().fireExportPresetEvent(ExportPresetEvent());
}

void MainWindow::saveAsPreset()
{
    EventManager::getInstance().fireSaveAsPresetEvent(SaveAsPresetEvent());
}

void MainWindow::newRundown()
{
    EventManager::getInstance().fireNewRundownEvent(NewRundownEvent());
}

void MainWindow::openRundown()
{
    EventManager::getInstance().fireOpenRundownEvent(OpenRundownEvent());
}

void MainWindow::openRundownFromUrl()
{
    EventManager::getInstance().fireOpenRundownFromUrlEvent(OpenRundownFromUrlEvent());
}

void MainWindow::saveRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(SaveRundownEvent(false));
}

void MainWindow::saveAsRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(SaveRundownEvent(true));
}

void MainWindow::executeStop()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier));
}

void MainWindow::executePlay()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier));
}

void MainWindow::executeLoad()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier));
}

void MainWindow::executePause()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier));
}

void MainWindow::executeNext()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F5, Qt::NoModifier));
}

void MainWindow::executeUpdate()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier));
}

void MainWindow::executeInvoke()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F7, Qt::NoModifier));
}

void MainWindow::executePreview()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F8, Qt::NoModifier));
}

void MainWindow::executeClear()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F10, Qt::NoModifier));
}

void MainWindow::executeClearVideolayer()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F11, Qt::NoModifier));
}

void MainWindow::executeClearChannel()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier));
}

void MainWindow::toggleCompactView()
{
    EventManager::getInstance().fireToggleCompactViewEvent(CompactViewEvent());
}

void MainWindow::allowRemoteTriggering(bool enabled)
{
    EventManager::getInstance().fireRemoteRundownTriggeringEvent(RemoteRundownTriggeringEvent(enabled));
    EventManager::getInstance().fireAllowRemoteTriggeringMenuEvent(AllowRemoteTriggeringMenuEvent(enabled));
}

void MainWindow::closeRundown()
{
    EventManager::getInstance().fireCloseRundownEvent(CloseRundownEvent());
}

void MainWindow::showAboutDialog()
{
    AboutDialog* dialog = new AboutDialog(this);
    dialog->exec();
}

void MainWindow::showHelpDialog()
{
    HelpDialog* dialog = new HelpDialog(this);
    dialog->exec();
}

void MainWindow::showSettingsDialog()
{
    // Reset inspector panel.
    EventManager::getInstance().fireEmptyRundownEvent(EmptyRundownEvent());

    SettingsDialog* dialog = new SettingsDialog(this);
    QObject::connect(dialog, SIGNAL(gpiBindingChanged(int, Playout::PlayoutType::Type)), this->widgetRundown, SLOT(gpiBindingChanged(int, Playout::PlayoutType::Type)));

    dialog->exec();
}

void MainWindow::toggleFullscreen()
{
    isFullScreen() ? setWindowState(Qt::WindowNoState) : setWindowState(Qt::WindowFullScreen);
}

void MainWindow::refreshLibrary()
{
    EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
}
