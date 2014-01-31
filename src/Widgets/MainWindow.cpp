#include "MainWindow.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include "Version.h"
#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"

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

    setWindowTitle(QString("%1 %2.%3.%4.%5").arg(this->windowTitle()).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION).arg(BUILD_VERSION));
    this->applicationTitle = this->windowTitle();

    this->widgetAction->setVisible(false);

    this->splitterHorizontal->setSizes(QList<int>() << 1 << 0);
    this->splitterVertical->setSizes(QList<int>() << 289 << 860 << 289);

    qApp->installEventFilter(this);
}

void MainWindow::setupMenu()
{
    this->fileMenu = new QMenu(this);
    this->fileMenu->addAction("New Rundown", this, SLOT(newRundown()));
    this->fileMenu->addAction("Open Rundown...", this, SLOT(openRundown()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Import Preset...", this, SLOT(importPreset()));
    this->fileMenu->addAction("Export Preset...", this, SLOT(exportPreset()));
    this->fileMenu->addAction("Save as Preset...", this, SLOT(saveAsPreset()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Save", this, SLOT(saveRundown()), QKeySequence::fromString("Ctrl+S"));
    this->fileMenu->addAction("Save As...", this, SLOT(saveAsRundown()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Quit", this, SLOT(close()));
    //this->fileMenu->actions().at(4)->setEnabled(false); // Export Preset...
    this->fileMenu->actions().at(5)->setEnabled(false); // Save as Preset...

    this->editMenu = new QMenu(this);
    this->editMenu->addAction("Settings...", this, SLOT(showSettingsDialog()));

    this->viewMenu = new QMenu(this);
    this->viewMenu->addAction("Toggle Fullscreen", this, SLOT(toggleFullscreen()), QKeySequence::fromString("Ctrl+F"));

    this->libraryMenu = new QMenu(this);
    this->libraryMenu->addAction("Refresh Library", this, SLOT(refreshLibrary()), QKeySequence::fromString("Ctrl+R"));

    this->rundownMenu = new QMenu(this);
    this->rundownMenu->addAction("Toggle Compact View", this, SLOT(toggleCompactView()), QKeySequence::fromString("Ctrl+W"));

    this->playoutMenu = new QMenu(this);
    this->playoutMenu->addAction("Stop", this, SLOT(executeStop()), QKeySequence::fromString("F1"));
    this->playoutMenu->addAction("Play", this, SLOT(executePlay()), QKeySequence::fromString("F2"));
    this->playoutMenu->addAction("Load", this, SLOT(executeLoad()), QKeySequence::fromString("F3"));
    this->playoutMenu->addAction("Pause", this, SLOT(executePause()), QKeySequence::fromString("F4"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Next", this, SLOT(executeNext()), QKeySequence::fromString("F5"));
    this->playoutMenu->addAction("Update", this, SLOT(executeUpdate()), QKeySequence::fromString("F6"));
    this->playoutMenu->addAction("Invoke", this, SLOT(executeInvoke()), QKeySequence::fromString("F7"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction("Clear", this, SLOT(executeClear()), QKeySequence::fromString("F10"));
    this->playoutMenu->addAction("Clear Video Layer", this, SLOT(executeClearVideolayer()), QKeySequence::fromString("F11"));
    this->playoutMenu->addAction("Clear Channel", this, SLOT(executeClearChannel()), QKeySequence::fromString("F12"));

    this->helpMenu = new QMenu(this);
    this->helpMenu->addAction("View Help", this, SLOT(showHelpDialog()), QKeySequence::fromString("Ctrl+H"));
    this->helpMenu->addSeparator();
    this->helpMenu->addAction("About CasparCG Client"/*, this, SLOT(showAboutDialog())*/);
    this->helpMenu->actions().at(2)->setEnabled(false); // About CasparCG Client

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

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::StatusbarMessage))
    {
        StatusbarEvent* statusbarEvent = dynamic_cast<StatusbarEvent*>(event);
        this->statusBar()->showMessage(statusbarEvent->getMessage(), statusbarEvent->getTimeout());
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ActiveRundownChanged))
    {
        ActiveRundownChangedEvent* activeRundownChangedEvent = dynamic_cast<ActiveRundownChangedEvent*>(event);

        QFileInfo info(activeRundownChangedEvent->getPath());
        if (info.baseName() == Rundown::DEFAULT_NAME)
            this->setWindowTitle(QString("%1").arg(this->applicationTitle));
        else
            this->setWindowTitle(QString("%1 - %2").arg(this->applicationTitle).arg(activeRundownChangedEvent->getPath()));

    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::NewRundownMenu))
    {
        NewRundownMenuEvent* newRundownMenuEvent = dynamic_cast<NewRundownMenuEvent*>(event);
        this->fileMenu->actions().at(0)->setEnabled(newRundownMenuEvent->getEnabled());
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        this->fileMenu->actions().at(5)->setEnabled(true);
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::EmptyRundown))
    {
        this->fileMenu->actions().at(5)->setEnabled(false);
    }


    return QObject::eventFilter(target, event);
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
    EventManager::getInstance().fireImportPresetEvent();
}

void MainWindow::exportPreset()
{
    EventManager::getInstance().fireExportPresetEvent();
}

void MainWindow::saveAsPreset()
{
    EventManager::getInstance().fireSaveAsPresetEvent();
}

void MainWindow::newRundown()
{
    EventManager::getInstance().fireNewRundownEvent();
}

void MainWindow::openRundown()
{
    EventManager::getInstance().fireOpenRundownEvent();
}

void MainWindow::saveRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(false);
}

void MainWindow::saveAsRundown()
{
    EventManager::getInstance().fireSaveRundownEvent(true);
}

void MainWindow::executeStop()
{
    EventManager::getInstance().fireExecuteStopEvent();
}

void MainWindow::executePlay()
{
    EventManager::getInstance().fireExecutePlayEvent();
}

void MainWindow::executePause()
{
    EventManager::getInstance().fireExecutePauseEvent();
}

void MainWindow::executeLoad()
{
    EventManager::getInstance().fireExecuteLoadEvent();
}

void MainWindow::executeNext()
{
    EventManager::getInstance().fireExecuteNextEvent();
}

void MainWindow::executeUpdate()
{
    EventManager::getInstance().fireExecuteUpdateEvent();
}

void MainWindow::executeInvoke()
{
    EventManager::getInstance().fireExecuteInvokeEvent();
}

void MainWindow::executeClear()
{
    EventManager::getInstance().fireExecuteClearEvent();
}

void MainWindow::executeClearVideolayer()
{
    EventManager::getInstance().fireExecuteClearVideolayerEvent();
}

void MainWindow::executeClearChannel()
{
    EventManager::getInstance().fireExecuteClearChannelEvent();
}

void MainWindow::toggleCompactView()
{
    EventManager::getInstance().fireToggleCompactViewEvent();
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
    EventManager::getInstance().fireEmptyRundownEvent();

    SettingsDialog* dialog = new SettingsDialog(this);
    QObject::connect(dialog, SIGNAL(gpiBindingChanged(int, Playout::PlayoutType::Type)), this->widgetRundown, SLOT(gpiBindingChanged(int, Playout::PlayoutType::Type)));

    dialog->exec();
}

void MainWindow::toggleFullscreen()
{
    isFullScreen() ? showNormal() : showFullScreen();
}

void MainWindow::refreshLibrary()
{
    EventManager::getInstance().fireRefreshLibraryEvent();
}
