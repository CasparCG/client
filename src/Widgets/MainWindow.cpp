#include "MainWindow.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include "Global.h"

#include "EventManager.h"
#include "Events/StatusbarEvent.h"
#include "Events/ActiveRundownChangedEvent.h"
#include "Events/NewRundownMenuEvent.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    setupMenu();
    setupToolbar();
    setWindowIcon(QIcon(":/Graphics/Images/CasparCG.png"));

    setWindowTitle(QString("%1 %2.%3 %4").arg(this->windowTitle()).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION));
    this->applicationTitle = this->windowTitle();

    this->widgetOnAirNow->setVisible(false);
    this->widgetClock->setVisible(false);
    this->widgetOnAirNext->setVisible(false);
    this->widgetTimeline->setVisible(false);
    this->splitterHorizontal->setSizes(QList<int>() << 0 << 1);

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
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Save", this, SLOT(saveRundown()), QKeySequence::fromString("Ctrl+S"));
    this->fileMenu->addAction("Save As...", this, SLOT(saveAsRundown()));
    this->fileMenu->addSeparator();
    this->fileMenu->addAction("Quit", this, SLOT(close()));

    this->editMenu = new QMenu(this);
    this->editMenu->addAction("Settings", this, SLOT(showSettingsDialog()));

    this->viewMenu = new QMenu(this);
    this->viewMenu->addAction("Toggle Fullscreen", this, SLOT(toggleFullscreen()), QKeySequence::fromString("Ctrl+F"));

    this->libraryMenu = new QMenu(this);
    this->libraryMenu->addAction("Refresh Library", this, SLOT(refreshLibrary()), QKeySequence::fromString("Ctrl+R"));

    this->rundownMenu = new QMenu(this);
    this->rundownMenu->addAction("Toggle Compact View", this, SLOT(toggleCompactView()), QKeySequence::fromString("Ctrl+W"));

    this->playoutMenu = new QMenu(this);
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlStopSmall.png"), "Stop", this, SLOT(executeStop()), QKeySequence::fromString("F1"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlPlaySmall.png"), "Play", this, SLOT(executePlay()), QKeySequence::fromString("F2"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlLoadSmall.png"), "Load", this, SLOT(executeLoad()), QKeySequence::fromString("F3"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlPauseSmall.png"), "Pause", this, SLOT(executePause()), QKeySequence::fromString("F4"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlNextSmall.png"), "Next", this, SLOT(executeNext()), QKeySequence::fromString("F5"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlUpdateSmall.png"), "Update", this, SLOT(executeUpdate()), QKeySequence::fromString("F6"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlUpdateSmall.png"), "Update", this, SLOT(executeInvoke()), QKeySequence::fromString("F7"));
    this->playoutMenu->addSeparator();
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlClearSmall.png"), "Clear", this, SLOT(executeClear()), QKeySequence::fromString("F10"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlClearVideolayerSmall.png"), "Clear Video Layer", this, SLOT(executeClearVideolayer()), QKeySequence::fromString("F11"));
    this->playoutMenu->addAction(QIcon(":/Graphics/Images/CtrlClearChannelSmall.png"), "Clear Channel", this, SLOT(executeClearChannel()), QKeySequence::fromString("F12"));

    this->helpMenu = new QMenu(this);
    this->helpMenu->addAction("View Help", this, SLOT(showHelpDialog()), QKeySequence::fromString("Ctrl+H"));
    this->helpMenu->addSeparator();
    this->helpMenu->addAction("About CasparCG Client");//, this, SLOT(showAboutDialog()));
    this->helpMenu->actions().at(2)->setEnabled(false);

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

void MainWindow::setupToolbar()
{
    this->toolBar = new QToolBar(this);
    this->toolBar->setMovable(false);
    this->toolBar->setFloatable(false);
    this->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    QToolButton* toolButtonCtrlStop = new QToolButton(this);
    toolButtonCtrlStop->setObjectName("toolButtonCtrlStop");
    toolButtonCtrlStop->setToolTip("Stop");

    QToolButton* toolButtonCtrlPlay = new QToolButton(this);
    toolButtonCtrlPlay->setObjectName("toolButtonCtrlPlay");
    toolButtonCtrlPlay->setToolTip("Play");

    QToolButton* toolButtonCtrlLoad = new QToolButton(this);
    toolButtonCtrlLoad->setObjectName("toolButtonCtrlLoad");
    toolButtonCtrlLoad->setToolTip("Load");

    QToolButton* toolButtonCtrlPause = new QToolButton(this);
    toolButtonCtrlPause->setObjectName("toolButtonCtrlPause");
    toolButtonCtrlPause->setToolTip("Pause");

    QToolButton* toolButtonCtrlNext = new QToolButton(this);
    toolButtonCtrlNext->setObjectName("toolButtonCtrlNext");
    toolButtonCtrlNext->setToolTip("Next");

    QToolButton* toolButtonCtrlUpdate = new QToolButton(this);
    toolButtonCtrlUpdate->setObjectName("toolButtonCtrlUpdate");
    toolButtonCtrlUpdate->setToolTip("Update");

    QToolButton* toolButtonCtrlInvoke = new QToolButton(this);
    toolButtonCtrlInvoke->setObjectName("toolButtonCtrlInvoke");
    toolButtonCtrlInvoke->setToolTip("Invoke");

    QToolButton* toolButtonCtrlClear = new QToolButton(this);
    toolButtonCtrlClear->setObjectName("toolButtonCtrlClear");
    toolButtonCtrlClear->setToolTip("Clear");

    QToolButton* toolButtonCtrlClearVideolayer = new QToolButton(this);
    toolButtonCtrlClearVideolayer->setObjectName("toolButtonCtrlClearVideolayer");
    toolButtonCtrlClearVideolayer->setToolTip("Clear Video Layer");

    QToolButton* toolButtonCtrlClearChannel = new QToolButton(this);
    toolButtonCtrlClearChannel->setObjectName("toolButtonCtrlClearChannel");
    toolButtonCtrlClearChannel->setToolTip("Clear Channel");

    QToolButton* toolButtonBlendMode = new QToolButton(this);
    toolButtonBlendMode->setObjectName("toolButtonBlendMode");
    toolButtonBlendMode->setToolTip("Blend Mode");

    QToolButton* toolButtonBrightness = new QToolButton(this);
    toolButtonBrightness->setObjectName("toolButtonBrightness");
    toolButtonBrightness->setToolTip("Brightness");

    QToolButton* toolButtonContrast = new QToolButton(this);
    toolButtonContrast->setObjectName("toolButtonContrast");
    toolButtonContrast->setToolTip("Contrast");

    QToolButton* toolButtonCrop = new QToolButton(this);
    toolButtonCrop->setObjectName("toolButtonCrop");
    toolButtonCrop->setToolTip("Crop");

    QToolButton* toolButtonGeometry = new QToolButton(this);
    toolButtonGeometry->setObjectName("toolButtonGeometry");
    toolButtonGeometry->setToolTip("Transformation");

    QToolButton* toolButtonGrid = new QToolButton(this);
    toolButtonGrid->setObjectName("toolButtonGrid");
    toolButtonGrid->setToolTip("Grid");

    QToolButton* toolButtonKeyer = new QToolButton(this);
    toolButtonKeyer->setObjectName("toolButtonKeyer");
    toolButtonKeyer->setToolTip("Mask");

    QToolButton* toolButtonLevels = new QToolButton(this);
    toolButtonLevels->setObjectName("toolButtonLevels");
    toolButtonLevels->setToolTip("Levels");

    QToolButton* toolButtonOpacity = new QToolButton(this);
    toolButtonOpacity->setObjectName("toolButtonOpacity");
    toolButtonOpacity->setToolTip("Opacity");

    QToolButton* toolButtonSaturation = new QToolButton(this);
    toolButtonSaturation->setObjectName("toolButtonSaturation");
    toolButtonSaturation->setToolTip("Saturation");

    QToolButton* toolButtonVolume = new QToolButton(this);
    toolButtonVolume->setObjectName("toolButtonVolume");
    toolButtonVolume->setToolTip("Volume");

    QToolButton* toolButtonCommit = new QToolButton(this);
    toolButtonCommit->setObjectName("toolButtonCommit");
    toolButtonCommit->setToolTip("Commit");

    QObject::connect(toolButtonBlendMode, SIGNAL(clicked()), this, SLOT(addBlendModeItem()));

    this->toolBar->addWidget(toolButtonCtrlStop);
    this->toolBar->addWidget(toolButtonCtrlPlay);
    this->toolBar->addWidget(toolButtonCtrlLoad);
    this->toolBar->addWidget(toolButtonCtrlPause);
    this->toolBar->addSeparator();
    this->toolBar->addWidget(toolButtonCtrlNext);
    this->toolBar->addWidget(toolButtonCtrlUpdate);
    this->toolBar->addWidget(toolButtonCtrlInvoke);
    this->toolBar->addSeparator();
    this->toolBar->addWidget(toolButtonCtrlClear);
    this->toolBar->addWidget(toolButtonCtrlClearVideolayer);
    this->toolBar->addWidget(toolButtonCtrlClearChannel);
    this->toolBar->addSeparator();
    this->toolBar->addWidget(toolButtonBlendMode);
    this->toolBar->addWidget(toolButtonBrightness);
    this->toolBar->addWidget(toolButtonContrast);
    this->toolBar->addWidget(toolButtonCrop);
    this->toolBar->addWidget(toolButtonGeometry);
    this->toolBar->addWidget(toolButtonGrid);
    this->toolBar->addWidget(toolButtonKeyer);
    this->toolBar->addWidget(toolButtonLevels);
    this->toolBar->addWidget(toolButtonOpacity);
    this->toolBar->addWidget(toolButtonSaturation);
    this->toolBar->addWidget(toolButtonVolume);
    this->toolBar->addWidget(toolButtonCommit);

    addToolBar(this->toolBar);
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::StatusbarMessage))
    {
        StatusbarEvent* statusbarEvent = dynamic_cast<StatusbarEvent*>(event);
        this->statusBar()->showMessage(statusbarEvent->getMessage(), statusbarEvent->getTimeout());
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::ActiveRundownChanged))
    {
        ActiveRundownChangedEvent* activeRundownChangedEvent = dynamic_cast<ActiveRundownChangedEvent*>(event);

        QFileInfo info(activeRundownChangedEvent->getPath());
        if (info.baseName() == Rundown::DEFAULT_NAME)
            this->setWindowTitle(QString("%1").arg(this->applicationTitle));
        else
            this->setWindowTitle(QString("%1 - %2").arg(this->applicationTitle).arg(activeRundownChangedEvent->getPath()));

    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::NewRundownMenu))
    {
        NewRundownMenuEvent* newRundownMenuEvent = dynamic_cast<NewRundownMenuEvent*>(event);
        this->fileMenu->actions().at(0)->setEnabled(newRundownMenuEvent->getEnabled());
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
    box.buttons().at(0)->setFocusPolicy(Qt::NoFocus);
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

void MainWindow::addBlendModeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::BLENDMODE);
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
