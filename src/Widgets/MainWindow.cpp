#include "MainWindow.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "SettingsDialog.h"

#include "Global.h"

#include "EventManager.h"
#include "DatabaseManager.h"
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
#include <QtGui/QToolButton>

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
    this->toolBar->setIconSize(QSize(28, 28));
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

    QToolButton* toolButtonAudio = new QToolButton(this);
    toolButtonAudio->setObjectName("toolButtonAudio");
    toolButtonAudio->setToolTip("Audio");

    QToolButton* toolButtonImage = new QToolButton(this);
    toolButtonImage->setObjectName("toolButtonImage");
    toolButtonImage->setToolTip("Image");

    QToolButton* toolButtonImageScroller = new QToolButton(this);
    toolButtonImageScroller->setObjectName("toolButtonImageScroller");
    toolButtonImageScroller->setToolTip("Image Scroller");

    QToolButton* toolButtonTemplate = new QToolButton(this);
    toolButtonTemplate->setObjectName("toolButtonTemplate");
    toolButtonTemplate->setToolTip("TEmplate");

    QToolButton* toolButtonVideo = new QToolButton(this);
    toolButtonVideo->setObjectName("toolButtonVideo");
    toolButtonVideo->setToolTip("Video");

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

    QToolButton* toolButtonSolidColor = new QToolButton(this);
    toolButtonSolidColor->setObjectName("toolButtonSolidColor");
    toolButtonSolidColor->setToolTip("Solid Color");

    QToolButton* toolButtonGpiOutput = new QToolButton(this);
    toolButtonGpiOutput->setObjectName("toolButtonGpiOutput");
    toolButtonGpiOutput->setToolTip("GPI Output");

    QToolButton* toolButtonFileRecorder = new QToolButton(this);
    toolButtonFileRecorder->setObjectName("toolButtonFileRecorder");
    toolButtonFileRecorder->setToolTip("File Recorder");

    QToolButton* toolButtonDeckLinkInput = new QToolButton(this);
    toolButtonDeckLinkInput->setObjectName("toolButtonDeckLinkInput");
    toolButtonDeckLinkInput->setToolTip("DeckLink Input");

    QToolButton* toolButtonChannelSnapshot = new QToolButton(this);
    toolButtonChannelSnapshot->setObjectName("toolButtonChannelSnapshot");
    toolButtonChannelSnapshot->setToolTip("Channel Snapshot");

    QToolButton* toolButtonClearOutput = new QToolButton(this);
    toolButtonClearOutput->setObjectName("toolButtonClearOutput");
    toolButtonClearOutput->setToolTip("Clear Output");

    QToolButton* toolButtonCustomCommand = new QToolButton(this);
    toolButtonCustomCommand->setObjectName("toolButtonCustomCommand");
    toolButtonCustomCommand->setToolTip("Custom Command");

    QObject::connect(toolButtonBlendMode, SIGNAL(clicked()), this, SLOT(addBlendModeItem()));
    QObject::connect(toolButtonBrightness, SIGNAL(clicked()), this, SLOT(addBrightnessItem()));
    QObject::connect(toolButtonContrast, SIGNAL(clicked()), this, SLOT(addContrastItem()));
    QObject::connect(toolButtonCrop, SIGNAL(clicked()), this, SLOT(addCropItem()));
    QObject::connect(toolButtonGeometry, SIGNAL(clicked()), this, SLOT(addGeometryItem()));
    QObject::connect(toolButtonGrid, SIGNAL(clicked()), this, SLOT(addGridItem()));
    QObject::connect(toolButtonKeyer, SIGNAL(clicked()), this, SLOT(addKeyerItem()));
    QObject::connect(toolButtonLevels, SIGNAL(clicked()), this, SLOT(addLevelsItem()));
    QObject::connect(toolButtonOpacity, SIGNAL(clicked()), this, SLOT(addOpacityItem()));
    QObject::connect(toolButtonSaturation, SIGNAL(clicked()), this, SLOT(addSaturationItem()));
    QObject::connect(toolButtonVolume, SIGNAL(clicked()), this, SLOT(addVolumeItem()));
    QObject::connect(toolButtonCommit, SIGNAL(clicked()), this, SLOT(addCommitItem()));
    QObject::connect(toolButtonAudio, SIGNAL(clicked()), this, SLOT(addAudioItem()));
    QObject::connect(toolButtonImage, SIGNAL(clicked()), this, SLOT(addImageItem()));
    QObject::connect(toolButtonImageScroller, SIGNAL(clicked()), this, SLOT(addImageScrollerItem()));
    QObject::connect(toolButtonTemplate, SIGNAL(clicked()), this, SLOT(addTemplateItem()));
    QObject::connect(toolButtonVideo, SIGNAL(clicked()), this, SLOT(addVideoItem()));
    QObject::connect(toolButtonSolidColor, SIGNAL(clicked()), this, SLOT(addSolidColorItem()));
    QObject::connect(toolButtonGpiOutput, SIGNAL(clicked()), this, SLOT(addGpiOutputItem()));
    QObject::connect(toolButtonFileRecorder, SIGNAL(clicked()), this, SLOT(addFileRecorderItem()));
    QObject::connect(toolButtonDeckLinkInput, SIGNAL(clicked()), this, SLOT(addDeckLinkInputItem()));
    QObject::connect(toolButtonChannelSnapshot, SIGNAL(clicked()), this, SLOT(addPrintItem()));
    QObject::connect(toolButtonClearOutput, SIGNAL(clicked()), this, SLOT(addClearOutputItem()));
    QObject::connect(toolButtonCustomCommand, SIGNAL(clicked()), this, SLOT(addCustomCommandItem()));
    QObject::connect(toolButtonCtrlStop, SIGNAL(clicked()), this, SLOT(executeStop()));
    QObject::connect(toolButtonCtrlPlay, SIGNAL(clicked()), this, SLOT(executePlay()));
    QObject::connect(toolButtonCtrlLoad, SIGNAL(clicked()), this, SLOT(executeLoad()));
    QObject::connect(toolButtonCtrlPause, SIGNAL(clicked()), this, SLOT(executePause()));
    QObject::connect(toolButtonCtrlNext, SIGNAL(clicked()), this, SLOT(executeNext()));
    QObject::connect(toolButtonCtrlUpdate, SIGNAL(clicked()), this, SLOT(executeUpdate()));
    QObject::connect(toolButtonCtrlInvoke, SIGNAL(clicked()), this, SLOT(executeInvoke()));
    QObject::connect(toolButtonCtrlClear, SIGNAL(clicked()), this, SLOT(executeClear()));
    QObject::connect(toolButtonCtrlClearVideolayer, SIGNAL(clicked()), this, SLOT(executeClearVideolayer()));
    QObject::connect(toolButtonCtrlClearChannel, SIGNAL(clicked()), this, SLOT(executeClearChannel()));

    this->toolBar->addWidget(toolButtonCtrlStop);
    this->toolBar->addWidget(toolButtonCtrlPlay);
    this->toolBar->addWidget(toolButtonCtrlLoad);
    this->toolBar->addWidget(toolButtonCtrlPause);
    this->toolBar->addWidget(toolButtonCtrlNext);
    this->toolBar->addWidget(toolButtonCtrlUpdate);
    this->toolBar->addWidget(toolButtonCtrlInvoke);
    this->toolBar->addWidget(toolButtonCtrlClear);
    this->toolBar->addWidget(toolButtonCtrlClearVideolayer);
    this->toolBar->addWidget(toolButtonCtrlClearChannel);
    this->toolBar->addSeparator();
    this->toolBar->addWidget(toolButtonAudio);
    this->toolBar->addWidget(toolButtonImage);
    this->toolBar->addWidget(toolButtonImageScroller);
    this->toolBar->addWidget(toolButtonTemplate);
    this->toolBar->addWidget(toolButtonVideo);
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
    this->toolBar->addSeparator();
    this->toolBar->addWidget(toolButtonSolidColor);
    this->toolBar->addWidget(toolButtonGpiOutput);
    this->toolBar->addWidget(toolButtonFileRecorder);
    this->toolBar->addWidget(toolButtonDeckLinkInput);
    this->toolBar->addWidget(toolButtonChannelSnapshot);
    this->toolBar->addWidget(toolButtonClearOutput);
    this->toolBar->addWidget(toolButtonCustomCommand);

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

void MainWindow::addBrightnessItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::BRIGHTNESS);
}

void MainWindow::addContrastItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CONTRAST);
}

void MainWindow::addCropItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CROP);
}

void MainWindow::addImageScrollerItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::IMAGESCROLLER);
}

void MainWindow::addDeckLinkInputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::DECKLINKINPUT);
}

void MainWindow::addPrintItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::PRINT);
}

void MainWindow::addClearOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CLEAROUTPUT);
}

void MainWindow::addGeometryItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GEOMETRY);
}

void MainWindow::addGpiOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GPIOUTPUT);
}

void MainWindow::addFileRecorderItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::FILERECORDER);
}

void MainWindow::addSeparatorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SEPARATOR);
}

void MainWindow::addGridItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GRID);
}

void MainWindow::addCustomCommandItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CUSTOMCOMMAND);
}

void MainWindow::addSolidColorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SOLIDCOLOR);
}

void MainWindow::addKeyerItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::KEYER);
}

void MainWindow::addLevelsItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::LEVELS);
}

void MainWindow::addOpacityItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::OPACITY);
}

void MainWindow::addSaturationItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SATURATION);
}

void MainWindow::addVolumeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::VOLUME);
}

void MainWindow::addCommitItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::COMMIT);
}

void MainWindow::addAudioItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::AUDIO);
}

void MainWindow::addImageItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::IMAGE);
}

void MainWindow::addTemplateItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TEMPLATE);
}

void MainWindow::addVideoItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::VIDEO);
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

void MainWindow::toggleToolbar()
{
    this->toolBar->setVisible(!this->toolBar->isVisible());
}

void MainWindow::refreshLibrary()
{
    EventManager::getInstance().fireRefreshLibraryEvent();
}
