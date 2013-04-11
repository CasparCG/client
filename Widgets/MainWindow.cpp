#include "MainWindow.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include "Global.h"

#include "EventManager.h"
#include "Events/StatusbarEvent.h"
#include "Events/WindowTitleEvent.h"

#include <QtCore/QDebug>

#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QMouseEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    setupUiMenu();
    setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));


    setWindowTitle(QString("%1 %2.%3 %4").arg(this->windowTitle()).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(TAG_VERSION));
    this->applicationTitle = this->windowTitle();

    this->widgetOnAirNow->setVisible(false);
    this->widgetClock->setVisible(false);
    this->widgetOnAirNext->setVisible(false);
    this->widgetTimeline->setVisible(false);
    this->splitterHorizontal->setSizes(QList<int>() << 0 << 1);

    qApp->installEventFilter(this);
}

void MainWindow::setupUiMenu()
{
    this->fileMenu = new QMenu(this);
    //this->fileMenu->addAction("New", this, SLOT(newRundown()));
    this->fileMenu->addAction("Open...", this, SLOT(openRundown()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Save", this, SLOT(saveRundown()), QKeySequence::fromString("Ctrl+S"));
    this->fileMenu->addAction("Save As...", this, SLOT(saveAsRundown()));
    //this->fileMenu->addSeparator();
    //this->fileMenu->addAction("Import");//, this, SLOT(showAboutDialog()), QKeySequence::fromString("Ctrl+I"));
    //this->fileMenu->addAction("Export");//, this, SLOT(showAboutDialog()), QKeySequence::fromString("Ctrl+E"));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Quit", this, SLOT(close()));
    //this->fileMenu->actions().at(3)->setEnabled(false);
    //this->fileMenu->actions().at(4)->setEnabled(false);

    this->editMenu = new QMenu(this);
    this->editMenu->addAction("Settings", this, SLOT(showSettingsDialog()));

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
    this->helpMenu->addAction("About CasparCG Client");//, this, SLOT(showAboutDialog()));
    this->helpMenu->actions().at(2)->setEnabled(false);

    this->menuBar = new QMenuBar(this);
    this->menuBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->menuBar->addMenu(this->fileMenu)->setText("File");
    this->menuBar->addMenu(this->editMenu)->setText("Edit");
    this->menuBar->addMenu(this->viewMenu)->setText("View");
    this->menuBar->addMenu(this->libraryMenu)->setText("Library");
    this->menuBar->addMenu(this->rundownMenu)->setText("Rundown");
    this->menuBar->addMenu(this->playoutMenu)->setText("Playout");
    this->menuBar->addMenu(this->helpMenu)->setText("Help");

    this->setMenuBar(this->menuBar);
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::StatusbarMessage))
    {
        StatusbarEvent* statusbarEvent = dynamic_cast<StatusbarEvent*>(event);
        statusBar()->showMessage(statusbarEvent->getMessage(), statusbarEvent->getTimeout());
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::WindowTitle))
    {
        WindowTitleEvent* windowTitleEvent = dynamic_cast<WindowTitleEvent*>(event);
        setWindowTitle(QString("%1 - %2").arg(this->applicationTitle).arg(windowTitleEvent->getTitle()));

    }

    return QObject::eventFilter(target, event);
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

    connect(dialog, SIGNAL(gpiBindingChanged(int, Playout::PlayoutType::Type)), this->widgetRundown, SLOT(gpiBindingChanged(int, Playout::PlayoutType::Type)));

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
