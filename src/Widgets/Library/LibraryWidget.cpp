#include "LibraryWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "DeviceFilterWidget.h"
#include "Events/AddPresetItemEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/Inspector/AddTemplateDataEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/PresetModel.h"

#include <QtCore/QPoint>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QTextStream>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtCore/QModelIndex>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStandardItemModel>

LibraryWidget::LibraryWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    setupUiMenu();
    setupTools();

    this->treeWidgetTool->setColumnHidden(1, true);
    this->treeWidgetTool->setColumnHidden(2, true);
    this->treeWidgetTool->setColumnHidden(3, true);
    this->treeWidgetTool->setColumnHidden(4, true);
    this->treeWidgetTool->setColumnHidden(5, true);
    this->treeWidgetTool->setColumnHidden(6, true);

    this->treeWidgetAudio->setColumnHidden(1, true);
    this->treeWidgetAudio->setColumnHidden(2, true);
    this->treeWidgetAudio->setColumnHidden(3, true);
    this->treeWidgetAudio->setColumnHidden(4, true);
    this->treeWidgetAudio->setColumnHidden(5, true);
    this->treeWidgetAudio->setColumnHidden(6, true);

    this->treeWidgetImage->setColumnHidden(1, true);
    this->treeWidgetImage->setColumnHidden(2, true);
    this->treeWidgetImage->setColumnHidden(3, true);
    this->treeWidgetImage->setColumnHidden(4, true);
    this->treeWidgetImage->setColumnHidden(5, true);
    this->treeWidgetImage->setColumnHidden(6, true);

    this->treeWidgetTemplate->setColumnHidden(1, true);
    this->treeWidgetTemplate->setColumnHidden(2, true);
    this->treeWidgetTemplate->setColumnHidden(3, true);
    this->treeWidgetTemplate->setColumnHidden(4, true);
    this->treeWidgetTemplate->setColumnHidden(5, true);
    this->treeWidgetTemplate->setColumnHidden(6, true);

    this->treeWidgetVideo->header()->setResizeMode(0, QHeaderView::Stretch);
    this->treeWidgetVideo->setColumnHidden(1, true);
    this->treeWidgetVideo->setColumnHidden(2, true);
    this->treeWidgetVideo->setColumnHidden(3, true);
    this->treeWidgetVideo->setColumnHidden(4, true);
    this->treeWidgetVideo->setColumnHidden(5, true);
    this->treeWidgetVideo->setColumnWidth(6, 65);

    this->treeWidgetData->setColumnHidden(1, true);
    this->treeWidgetData->setColumnHidden(2, true);
    this->treeWidgetData->setColumnHidden(3, true);
    this->treeWidgetData->setColumnHidden(4, true);
    this->treeWidgetData->setColumnHidden(5, true);
    this->treeWidgetData->setColumnHidden(6, true);

    this->treeWidgetPreset->setColumnHidden(1, true);
    this->treeWidgetPreset->setColumnHidden(2, true);

    QObject::connect(this->treeWidgetTool, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetPreset, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuPresetRequested(const QPoint &)));
    QObject::connect(this->treeWidgetAudio, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetImage, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuImageRequested(const QPoint &)));
    QObject::connect(this->treeWidgetTemplate, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetVideo, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));
    QObject::connect(this->treeWidgetData, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuDataRequested(const QPoint &)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(mediaChanged(const MediaChangedEvent&)), this, SLOT(mediaChanged(const MediaChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(templateChanged(const TemplateChangedEvent&)), this, SLOT(templateChanged(const TemplateChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(dataChanged(const DataChangedEvent&)), this, SLOT(dataChanged(const DataChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(presetChanged(const PresetChangedEvent&)), this, SLOT(presetChanged(const PresetChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(importPreset(const ImportPresetEvent&)), this, SLOT(importPreset(const ImportPresetEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(exportPreset(const ExportPresetEvent&)), this, SLOT(exportPreset(const ExportPresetEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(repositoryRundown(const RepositoryRundownEvent&)), this, SLOT(repositoryRundown(const RepositoryRundownEvent&)));

    QTimer::singleShot(0, this, SLOT(loadLibrary()));
}

void LibraryWidget::setupTools()
{
    QTreeWidgetItem* widgetAudio = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetAudio->setIcon(0, QIcon(":/Graphics/Images/AudioSmall.png"));
    widgetAudio->setText(0, "Audio");
    widgetAudio->setText(1, "0");
    widgetAudio->setText(2, "Audio");
    widgetAudio->setText(3, "");
    widgetAudio->setText(4, Rundown::AUDIO);
    widgetAudio->setText(5, "0");
    widgetAudio->setText(6, "");

    QTreeWidgetItem* widgetStill = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetStill->setIcon(0, QIcon(":/Graphics/Images/StillSmall.png"));
    widgetStill->setText(0, "Image");
    widgetStill->setText(1, "0");
    widgetStill->setText(2, "Image");
    widgetStill->setText(3, "");
    widgetStill->setText(4, Rundown::STILL);
    widgetStill->setText(5, "0");
    widgetStill->setText(6, "");

    QTreeWidgetItem* widgetImageScroller = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetImageScroller->setIcon(0, QIcon(":/Graphics/Images/ImageScrollerSmall.png"));
    widgetImageScroller->setText(0, "Image Scroller");
    widgetImageScroller->setText(1, "0");
    widgetImageScroller->setText(2, "Image Scroller");
    widgetImageScroller->setText(3, "");
    widgetImageScroller->setText(4, Rundown::IMAGESCROLLER);
    widgetImageScroller->setText(5, "0");
    widgetImageScroller->setText(6, "");

    QTreeWidgetItem* widgetTemplate = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetTemplate->setIcon(0, QIcon(":/Graphics/Images/TemplateSmall.png"));
    widgetTemplate->setText(0, "Template");
    widgetTemplate->setText(1, "0");
    widgetTemplate->setText(2, "Template");
    widgetTemplate->setText(3, "");
    widgetTemplate->setText(4, Rundown::TEMPLATE);
    widgetTemplate->setText(5, "0");
    widgetTemplate->setText(6, "");

    QTreeWidgetItem* widgetMovie = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetMovie->setIcon(0, QIcon(":/Graphics/Images/MovieSmall.png"));
    widgetMovie->setText(0, "Video");
    widgetMovie->setText(1, "0");
    widgetMovie->setText(2, "Video");
    widgetMovie->setText(3, "");
    widgetMovie->setText(4, Rundown::MOVIE);
    widgetMovie->setText(5, "0");
    widgetMovie->setText(6, "");

    QTreeWidgetItem* widgetAnchor = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetAnchor->setIcon(0, QIcon(":/Graphics/Images/AnchorSmall.png"));
    widgetAnchor->setText(0, "Anchor Point");
    widgetAnchor->setText(1, "0");
    widgetAnchor->setText(2, "Anchor Point");
    widgetAnchor->setText(3, "");
    widgetAnchor->setText(4, Rundown::ANCHOR);
    widgetAnchor->setText(5, "0");

    QTreeWidgetItem* widgetBlendMode = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetBlendMode->setIcon(0, QIcon(":/Graphics/Images/BlendModeSmall.png"));
    widgetBlendMode->setText(0, "Blend Mode");
    widgetBlendMode->setText(1, "0");
    widgetBlendMode->setText(2, "Blend Mode");
    widgetBlendMode->setText(3, "");
    widgetBlendMode->setText(4, Rundown::BLENDMODE);
    widgetBlendMode->setText(5, "0");

    QTreeWidgetItem* widgetBrightness = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetBrightness->setIcon(0, QIcon(":/Graphics/Images/BrightnessSmall.png"));
    widgetBrightness->setText(0, "Brightness");
    widgetBrightness->setText(1, "0");
    widgetBrightness->setText(2, "Brightness");
    widgetBrightness->setText(3, "");
    widgetBrightness->setText(4, Rundown::BRIGHTNESS);
    widgetBrightness->setText(5, "0");

    QTreeWidgetItem* widgetChroma = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetChroma->setIcon(0, QIcon(":/Graphics/Images/ChromaSmall.png"));
    widgetChroma->setText(0, "Chroma Key");
    widgetChroma->setText(1, "0");
    widgetChroma->setText(2, "Chroma Key");
    widgetChroma->setText(3, "");
    widgetChroma->setText(4, Rundown::CHROMAKEY);
    widgetChroma->setText(5, "0");

    QTreeWidgetItem* widgetClip = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetClip->setIcon(0, QIcon(":/Graphics/Images/ClipSmall.png"));
    widgetClip->setText(0, "Clipping");
    widgetClip->setText(1, "0");
    widgetClip->setText(2, "Clipping");
    widgetClip->setText(3, "");
    widgetClip->setText(4, Rundown::CLIP);
    widgetClip->setText(5, "0");

    QTreeWidgetItem* widgetCommit = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetCommit->setIcon(0, QIcon(":/Graphics/Images/CommitSmall.png"));
    widgetCommit->setText(0, "Commit");
    widgetCommit->setText(1, "0");
    widgetCommit->setText(2, "Commit");
    widgetCommit->setText(3, "");
    widgetCommit->setText(4, Rundown::COMMIT);
    widgetCommit->setText(5, "0");

    QTreeWidgetItem* widgetContrast = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetContrast->setIcon(0, QIcon(":/Graphics/Images/ContrastSmall.png"));
    widgetContrast->setText(0, "Contrast");
    widgetContrast->setText(1, "0");
    widgetContrast->setText(2, "Contrast");
    widgetContrast->setText(3, "");
    widgetContrast->setText(4, Rundown::CONTRAST);
    widgetContrast->setText(5, "0");

    QTreeWidgetItem* widgetCrop = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetCrop->setIcon(0, QIcon(":/Graphics/Images/CropSmall.png"));
    widgetCrop->setText(0, "Crop");
    widgetCrop->setText(1, "0");
    widgetCrop->setText(2, "Crop");
    widgetCrop->setText(3, "");
    widgetCrop->setText(4, Rundown::CROP);
    widgetCrop->setText(5, "0");

    QTreeWidgetItem* widgetPerspective = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetPerspective->setIcon(0, QIcon(":/Graphics/Images/PerspectiveSmall.png"));
    widgetPerspective->setText(0, "Distort");
    widgetPerspective->setText(1, "0");
    widgetPerspective->setText(2, "Distort");
    widgetPerspective->setText(3, "");
    widgetPerspective->setText(4, Rundown::PERSPECTIVE);
    widgetPerspective->setText(5, "0");

    QTreeWidgetItem* widgetGrid = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetGrid->setIcon(0, QIcon(":/Graphics/Images/GridSmall.png"));
    widgetGrid->setText(0, "Grid");
    widgetGrid->setText(1, "0");
    widgetGrid->setText(2, "Grid");
    widgetGrid->setText(3, "");
    widgetGrid->setText(4, Rundown::GRID);
    widgetGrid->setText(5, "0");

    QTreeWidgetItem* widgetLevels = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetLevels->setIcon(0, QIcon(":/Graphics/Images/LevelsSmall.png"));
    widgetLevels->setText(0, "Levels");
    widgetLevels->setText(1, "0");
    widgetLevels->setText(2, "Levels");
    widgetLevels->setText(3, "");
    widgetLevels->setText(4, Rundown::LEVELS);
    widgetLevels->setText(5, "0");

    QTreeWidgetItem* widgetKeyer = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetKeyer->setIcon(0, QIcon(":/Graphics/Images/KeyerSmall.png"));
    widgetKeyer->setText(0, "Mask");
    widgetKeyer->setText(1, "0");
    widgetKeyer->setText(2, "Mask");
    widgetKeyer->setText(3, "");
    widgetKeyer->setText(4, Rundown::KEYER);
    widgetKeyer->setText(5, "0");

    QTreeWidgetItem* widgetOpacity = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetOpacity->setIcon(0, QIcon(":/Graphics/Images/OpacitySmall.png"));
    widgetOpacity->setText(0, "Opacity");
    widgetOpacity->setText(1, "0");
    widgetOpacity->setText(2, "Opacity");
    widgetOpacity->setText(3, "");
    widgetOpacity->setText(4, Rundown::OPACITY);
    widgetOpacity->setText(5, "0");

    QTreeWidgetItem* widgetReset = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetReset->setIcon(0, QIcon(":/Graphics/Images/ResetSmall.png"));
    widgetReset->setText(0, "Reset");
    widgetReset->setText(1, "0");
    widgetReset->setText(2, "Reset");
    widgetReset->setText(3, "");
    widgetReset->setText(4, Rundown::RESET);
    widgetReset->setText(5, "0");

    QTreeWidgetItem* widgetRotation = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetRotation->setIcon(0, QIcon(":/Graphics/Images/RotationSmall.png"));
    widgetRotation->setText(0, "Rotation");
    widgetRotation->setText(1, "0");
    widgetRotation->setText(2, "Rotation");
    widgetRotation->setText(3, "");
    widgetRotation->setText(4, Rundown::ROTATION);
    widgetRotation->setText(5, "0");

    QTreeWidgetItem* widgetSaturation = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetSaturation->setIcon(0, QIcon(":/Graphics/Images/SaturationSmall.png"));
    widgetSaturation->setText(0, "Saturation");
    widgetSaturation->setText(1, "0");
    widgetSaturation->setText(2, "Saturation");
    widgetSaturation->setText(3, "");
    widgetSaturation->setText(4, Rundown::SATURATION);
    widgetSaturation->setText(5, "0");

    QTreeWidgetItem* widgetFill = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetFill->setIcon(0, QIcon(":/Graphics/Images/FillSmall.png"));
    widgetFill->setText(0, "Transform");
    widgetFill->setText(1, "0");
    widgetFill->setText(2, "Transform");
    widgetFill->setText(3, "");
    widgetFill->setText(4, Rundown::FILL);
    widgetFill->setText(5, "0");

    QTreeWidgetItem* widgetVolume = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetVolume->setIcon(0, QIcon(":/Graphics/Images/VolumeSmall.png"));
    widgetVolume->setText(0, "Volume");
    widgetVolume->setText(1, "0");
    widgetVolume->setText(2, "Volume");
    widgetVolume->setText(3, "");
    widgetVolume->setText(4, Rundown::VOLUME);
    widgetVolume->setText(5, "0");

    QTreeWidgetItem* widgetChannelSnapshot = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetChannelSnapshot->setIcon(0, QIcon(":/Graphics/Images/SnapshotSmall.png"));
    widgetChannelSnapshot->setText(0, "Channel Snapshot");
    widgetChannelSnapshot->setText(1, "0");
    widgetChannelSnapshot->setText(2, "Channel Snapshot");
    widgetChannelSnapshot->setText(3, "");
    widgetChannelSnapshot->setText(4, Rundown::PRINT);
    widgetChannelSnapshot->setText(5, "0");

    QTreeWidgetItem* widgetClearOutput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetClearOutput->setIcon(0, QIcon(":/Graphics/Images/ClearSmall.png"));
    widgetClearOutput->setText(0, "Clear Output");
    widgetClearOutput->setText(1, "0");
    widgetClearOutput->setText(2, "Clear Output");
    widgetClearOutput->setText(3, "");
    widgetClearOutput->setText(4, Rundown::CLEAROUTPUT);
    widgetClearOutput->setText(5, "0");

    QTreeWidgetItem* widgetCustomCommand = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetCustomCommand->setIcon(0, QIcon(":/Graphics/Images/CustomCommandSmall.png"));
    widgetCustomCommand->setText(0, "Custom Command");
    widgetCustomCommand->setText(1, "0");
    widgetCustomCommand->setText(2, "Custom Command");
    widgetCustomCommand->setText(3, "");
    widgetCustomCommand->setText(4, Rundown::CUSTOMCOMMAND);
    widgetCustomCommand->setText(5, "0");

    QTreeWidgetItem* widgetDeckLinkInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetDeckLinkInput->setIcon(0, QIcon(":/Graphics/Images/DeckLinkProducerSmall.png"));
    widgetDeckLinkInput->setText(0, "DeckLink Input");
    widgetDeckLinkInput->setText(1, "0");
    widgetDeckLinkInput->setText(2, "DeckLink Input");
    widgetDeckLinkInput->setText(3, "");
    widgetDeckLinkInput->setText(4, Rundown::DECKLINKINPUT);
    widgetDeckLinkInput->setText(5, "0");

    QTreeWidgetItem* widgetFadeToBlack = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetFadeToBlack->setIcon(0, QIcon(":/Graphics/Images/FadeToBlackSmall.png"));
    widgetFadeToBlack->setText(0, "Fade to Black");
    widgetFadeToBlack->setText(1, "0");
    widgetFadeToBlack->setText(2, "Fade to Black");
    widgetFadeToBlack->setText(3, "");
    widgetFadeToBlack->setText(4, Rundown::FADETOBLACK);
    widgetFadeToBlack->setText(5, "0");

    QTreeWidgetItem* widgetFileRecorder = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetFileRecorder->setIcon(0, QIcon(":/Graphics/Images/FileRecorderSmall.png"));
    widgetFileRecorder->setText(0, "File Recorder");
    widgetFileRecorder->setText(1, "0");
    widgetFileRecorder->setText(2, "File Recorder");
    widgetFileRecorder->setText(3, "");
    widgetFileRecorder->setText(4, Rundown::FILERECORDER);
    widgetFileRecorder->setText(5, "0");

    QTreeWidgetItem* widgetGpiOutput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetGpiOutput->setIcon(0, QIcon(":/Graphics/Images/GpiOutputSmall.png"));
    widgetGpiOutput->setText(0, "GPI Output");
    widgetGpiOutput->setText(1, "0");
    widgetGpiOutput->setText(2, "GPI Output");
    widgetGpiOutput->setText(3, "");
    widgetGpiOutput->setText(4, Rundown::GPIOUTPUT);
    widgetGpiOutput->setText(5, "0");

    QTreeWidgetItem* widgetHtml = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetHtml->setIcon(0, QIcon(":/Graphics/Images/HtmlSmall.png"));
    widgetHtml->setText(0, "HTML Page");
    widgetHtml->setText(1, "0");
    widgetHtml->setText(2, "HTML Page");
    widgetHtml->setText(3, "");
    widgetHtml->setText(4, Rundown::HTML);
    widgetHtml->setText(5, "0");

    QTreeWidgetItem* widgetHttpGet = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetHttpGet->setIcon(0, QIcon(":/Graphics/Images/HttpGetSmall.png"));
    widgetHttpGet->setText(0, "HTTP GET Request");
    widgetHttpGet->setText(1, "0");
    widgetHttpGet->setText(2, "HTTP GET Request");
    widgetHttpGet->setText(3, "");
    widgetHttpGet->setText(4, Rundown::HTTPGET);
    widgetHttpGet->setText(5, "0");

    QTreeWidgetItem* widgetHttpPost = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetHttpPost->setIcon(0, QIcon(":/Graphics/Images/HttpPostSmall.png"));
    widgetHttpPost->setText(0, "HTTP POST Request");
    widgetHttpPost->setText(1, "0");
    widgetHttpPost->setText(2, "HTTP POST Request");
    widgetHttpPost->setText(3, "");
    widgetHttpPost->setText(4, Rundown::HTTPPOST);
    widgetHttpPost->setText(5, "0");

    QTreeWidgetItem* widgetOscOutput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetOscOutput->setIcon(0, QIcon(":/Graphics/Images/OscOutputSmall.png"));
    widgetOscOutput->setText(0, "OSC Output");
    widgetOscOutput->setText(1, "0");
    widgetOscOutput->setText(2, "OSC Output");
    widgetOscOutput->setText(3, "");
    widgetOscOutput->setText(4, Rundown::OSCOUTPUT);
    widgetOscOutput->setText(5, "0");

    QTreeWidgetItem* widgetPlayoutCommand = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetPlayoutCommand->setIcon(0, QIcon(":/Graphics/Images/PlayoutCommandSmall.png"));
    widgetPlayoutCommand->setText(0, "Playout Command");
    widgetPlayoutCommand->setText(1, "0");
    widgetPlayoutCommand->setText(2, "Playout Command");
    widgetPlayoutCommand->setText(3, "");
    widgetPlayoutCommand->setText(4, Rundown::PLAYOUTCOMMAND);
    widgetPlayoutCommand->setText(5, "0");

    QTreeWidgetItem* widgetRouteChannel = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetRouteChannel->setIcon(0, QIcon(":/Graphics/Images/RouteChannelSmall.png"));
    widgetRouteChannel->setText(0, "Route Channel");
    widgetRouteChannel->setText(1, "0");
    widgetRouteChannel->setText(2, "Route Channel");
    widgetRouteChannel->setText(3, "");
    widgetRouteChannel->setText(4, Rundown::ROUTECHANNEL);
    widgetRouteChannel->setText(5, "0");

    QTreeWidgetItem* widgetRouteVideolayer = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetRouteVideolayer->setIcon(0, QIcon(":/Graphics/Images/RouteVideolayerSmall.png"));
    widgetRouteVideolayer->setText(0, "Route Video Layer");
    widgetRouteVideolayer->setText(1, "0");
    widgetRouteVideolayer->setText(2, "Route Video Layer");
    widgetRouteVideolayer->setText(3, "");
    widgetRouteVideolayer->setText(4, Rundown::ROUTEVIDEOLAYER);
    widgetRouteVideolayer->setText(5, "0");

    QTreeWidgetItem* widgetSeparator = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetSeparator->setIcon(0, QIcon(":/Graphics/Images/SeparatorSmall.png"));
    widgetSeparator->setText(0, "Separator");
    widgetSeparator->setText(1, "0");
    widgetSeparator->setText(2, "");
    widgetSeparator->setText(3, "");
    widgetSeparator->setText(4, Rundown::SEPARATOR);
    widgetSeparator->setText(5, "0");

    QTreeWidgetItem* widgetSolidColor = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetSolidColor->setIcon(0, QIcon(":/Graphics/Images/SolidColorSmall.png"));
    widgetSolidColor->setText(0, "Solid Color");
    widgetSolidColor->setText(1, "0");
    widgetSolidColor->setText(2, "Solid Color");
    widgetSolidColor->setText(3, "");
    widgetSolidColor->setText(4, Rundown::SOLIDCOLOR);
    widgetSolidColor->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterMacro = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterMacro->setIcon(0, QIcon(":/Graphics/Images/TriCaster/PlayMacroSmall.png"));
    widgetTriCasterMacro->setText(0, "Play Macro");
    widgetTriCasterMacro->setText(1, "0");
    widgetTriCasterMacro->setText(2, "Play Macro");
    widgetTriCasterMacro->setText(3, "");
    widgetTriCasterMacro->setText(4, Rundown::TRICASTERMACRO);
    widgetTriCasterMacro->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterInput->setIcon(0, QIcon(":/Graphics/Images/TriCaster/SelectInputSmall.png"));
    widgetTriCasterInput->setText(0, "Select Input");
    widgetTriCasterInput->setText(1, "0");
    widgetTriCasterInput->setText(2, "Select Input");
    widgetTriCasterInput->setText(3, "");
    widgetTriCasterInput->setText(4, Rundown::TRICASTERINPUT);
    widgetTriCasterInput->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterSource = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterSource->setIcon(0, QIcon(":/Graphics/Images/TriCaster/SelectNetworkSourceSmall.png"));
    widgetTriCasterSource->setText(0, "Select Network Source");
    widgetTriCasterSource->setText(1, "0");
    widgetTriCasterSource->setText(2, "Select Network Source");
    widgetTriCasterSource->setText(3, "");
    widgetTriCasterSource->setText(4, Rundown::TRICASTERNETSOURCE);
    widgetTriCasterSource->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterPreset = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterPreset->setIcon(0, QIcon(":/Graphics/Images/TriCaster/SelectPresetSmall.png"));
    widgetTriCasterPreset->setText(0, "Select Preset");
    widgetTriCasterPreset->setText(1, "0");
    widgetTriCasterPreset->setText(2, "Select Preset");
    widgetTriCasterPreset->setText(3, "");
    widgetTriCasterPreset->setText(4, Rundown::TRICASTERPRESET);
    widgetTriCasterPreset->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterAuto = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterAuto->setIcon(0, QIcon(":/Graphics/Images/TriCaster/TriggerAutoSmall.png"));
    widgetTriCasterAuto->setText(0, "Trigger Auto");
    widgetTriCasterAuto->setText(1, "0");
    widgetTriCasterAuto->setText(2, "Trigger Auto");
    widgetTriCasterAuto->setText(3, "");
    widgetTriCasterAuto->setText(4, Rundown::TRICASTERAUTO);
    widgetTriCasterAuto->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterTake = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterTake->setIcon(0, QIcon(":/Graphics/Images/TriCaster/TriggerTakeSmall.png"));
    widgetTriCasterTake->setText(0, "Trigger Take");
    widgetTriCasterTake->setText(1, "0");
    widgetTriCasterTake->setText(2, "Trigger Take");
    widgetTriCasterTake->setText(3, "");
    widgetTriCasterTake->setText(4, Rundown::TRICASTERTAKE);
    widgetTriCasterTake->setText(5, "0");

    QTreeWidgetItem* widgetAtemInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemInput->setIcon(0, QIcon(":/Graphics/Images/Atem/SelectInputSmall.png"));
    widgetAtemInput->setText(0, "Select Input");
    widgetAtemInput->setText(1, "0");
    widgetAtemInput->setText(2, "Select Input");
    widgetAtemInput->setText(3, "");
    widgetAtemInput->setText(4, Rundown::ATEMINPUT);
    widgetAtemInput->setText(5, "0");

    QTreeWidgetItem* widgetAtemAudioGain = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemAudioGain->setIcon(0, QIcon(":/Graphics/Images/Atem/AudioGainSmall.png"));
    widgetAtemAudioGain->setText(0, "Set Audio Gain");
    widgetAtemAudioGain->setText(1, "0");
    widgetAtemAudioGain->setText(2, "Set Audio Gain");
    widgetAtemAudioGain->setText(3, "");
    widgetAtemAudioGain->setText(4, Rundown::ATEMAUDIOGAIN);
    widgetAtemAudioGain->setText(5, "0");

    QTreeWidgetItem* widgetAtemAudioInputState = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemAudioInputState->setIcon(0, QIcon(":/Graphics/Images/Atem/AudioInputStateSmall.png"));
    widgetAtemAudioInputState->setText(0, "Set Audio Input State");
    widgetAtemAudioInputState->setText(1, "0");
    widgetAtemAudioInputState->setText(2, "Set Audio Input State");
    widgetAtemAudioInputState->setText(3, "");
    widgetAtemAudioInputState->setText(4, Rundown::ATEMAUDIOINPUTSTATE);
    widgetAtemAudioInputState->setText(5, "0");

    QTreeWidgetItem* widgetAtemAudioInputBalance = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemAudioInputBalance->setIcon(0, QIcon(":/Graphics/Images/Atem/AudioInputBalanceSmall.png"));
    widgetAtemAudioInputBalance->setText(0, "Set Audio Input Balance");
    widgetAtemAudioInputBalance->setText(1, "0");
    widgetAtemAudioInputBalance->setText(2, "Set Audio Input Balance");
    widgetAtemAudioInputBalance->setText(3, "");
    widgetAtemAudioInputBalance->setText(4, Rundown::ATEMAUDIOINPUTBALANCE);
    widgetAtemAudioInputBalance->setText(5, "0");

    QTreeWidgetItem* widgetAtemKeyerState = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemKeyerState->setIcon(0, QIcon(":/Graphics/Images/Atem/KeyerStateSmall.png"));
    widgetAtemKeyerState->setText(0, "Set Keyer State");
    widgetAtemKeyerState->setText(1, "0");
    widgetAtemKeyerState->setText(2, "Set Keyer State");
    widgetAtemKeyerState->setText(3, "");
    widgetAtemKeyerState->setText(4, Rundown::ATEMKEYERSTATE);
    widgetAtemKeyerState->setText(5, "0");

    QTreeWidgetItem* widgetAtemVideoFormat = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemVideoFormat->setIcon(0, QIcon(":/Graphics/Images/Atem/VideoFormatSmall.png"));
    widgetAtemVideoFormat->setText(0, "Set Video Format");
    widgetAtemVideoFormat->setText(1, "0");
    widgetAtemVideoFormat->setText(2, "Set Video Format");
    widgetAtemVideoFormat->setText(3, "");
    widgetAtemVideoFormat->setText(4, Rundown::ATEMVIDEOFORMAT);
    widgetAtemVideoFormat->setText(5, "0");

    QTreeWidgetItem* widgetAtemAuto = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemAuto->setIcon(0, QIcon(":/Graphics/Images/Atem/TriggerAutoSmall.png"));
    widgetAtemAuto->setText(0, "Trigger Auto");
    widgetAtemAuto->setText(1, "0");
    widgetAtemAuto->setText(2, "Trigger Auto");
    widgetAtemAuto->setText(3, "");
    widgetAtemAuto->setText(4, Rundown::ATEMAUTO);
    widgetAtemAuto->setText(5, "0");

    QTreeWidgetItem* widgetAtemCut = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(4));
    widgetAtemCut->setIcon(0, QIcon(":/Graphics/Images/Atem/TriggerCutSmall.png"));
    widgetAtemCut->setText(0, "Trigger Cut");
    widgetAtemCut->setText(1, "0");
    widgetAtemCut->setText(2, "Trigger Cut");
    widgetAtemCut->setText(3, "");
    widgetAtemCut->setText(4, Rundown::ATEMCUT);
    widgetAtemCut->setText(5, "0");

    QTreeWidgetItem* widgetCameraPreset = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(5));
    widgetCameraPreset->setIcon(0, QIcon(":/Graphics/Images/Panasonic/PanasonicPresetSmall.png"));
    widgetCameraPreset->setText(0, "Camera Preset");
    widgetCameraPreset->setText(1, "0");
    widgetCameraPreset->setText(2, "Camera Preset");
    widgetCameraPreset->setText(3, "");
    widgetCameraPreset->setText(4, Rundown::PANASONICPRESET);
    widgetCameraPreset->setText(5, "0");

    this->treeWidgetTool->expandAll();
}

void LibraryWidget::setupUiMenu()
{
    this->contextMenu = new QMenu(this);
    this->contextMenu->addAction("Add item");

    this->contextMenuData = new QMenu(this);
    this->contextMenuData->addAction("Add stored data");

    this->contextMenuImage = new QMenu(this);
    this->contextMenuImage->addAction("Add image");
    this->contextMenuImage->addAction("Add as image scroller");

    this->contextMenuPreset = new QMenu(this);
    this->contextMenuPreset->addAction("Add item");
    this->contextMenuPreset->addSeparator();
    this->contextMenuPreset->addAction("Delete");

    QObject::connect(this->contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuTriggered(QAction*)));
    QObject::connect(this->contextMenuImage, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuImageTriggered(QAction*)));
    QObject::connect(this->contextMenuPreset, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuPresetTriggered(QAction*)));
    QObject::connect(this->contextMenuData, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuDataTriggered(QAction*)));
}

void LibraryWidget::repositoryRundown(const RepositoryRundownEvent& event)
{
    QTime time;
    time.start();

    for (int i = 0; i < this->treeWidgetTool->topLevelItemCount(); i++)
        this->treeWidgetTool->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetAudio->topLevelItemCount(); i++)
        this->treeWidgetAudio->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetImage->topLevelItemCount(); i++)
        this->treeWidgetImage->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetTemplate->topLevelItemCount(); i++)
        this->treeWidgetTemplate->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetVideo->topLevelItemCount(); i++)
        this->treeWidgetVideo->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetData->topLevelItemCount(); i++)
        this->treeWidgetData->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    for (int i = 0; i < this->treeWidgetPreset->topLevelItemCount(); i++)
        this->treeWidgetPreset->topLevelItem(i)->setDisabled(event.getRepositoryRundown());

    qDebug() << QString("LibraryWidget::repositoryRundown: Completed in %1 msec").arg(time.elapsed());
}

void LibraryWidget::mediaChanged(const MediaChangedEvent& event)
{
    // TODO: Only add / remove necessary items.
    this->treeWidgetAudio->clear();
    this->treeWidgetImage->clear();
    this->treeWidgetVideo->clear();
    this->treeWidgetAudio->clearSelection();
    this->treeWidgetImage->clearSelection();
    this->treeWidgetVideo->clearSelection();

    QList<LibraryModel> models;
    if (this->lineEditFilter->text().isEmpty() &&  dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter().count() == 0)
        models = DatabaseManager::getInstance().getLibraryMedia();
    else
        models = DatabaseManager::getInstance().getLibraryMediaByFilter(this->lineEditFilter->text(), dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter());

    if (models.count() > 0)
    {
        foreach (LibraryModel model, models)
        {
            if (model.getType() == "AUDIO")
            {
                QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetAudio);
                widget->setIcon(0, QIcon(":/Graphics/Images/AudioSmall.png"));
                widget->setText(0, model.getName());
                widget->setText(1, QString("%1").arg(model.getId()));
                widget->setText(2, model.getLabel());
                widget->setText(3, model.getDeviceName());
                widget->setText(4, model.getType());
                widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                widget->setText(6, model.getTimecode());
            }
            else if (model.getType() == "STILL")
            {
                QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetImage);
                widget->setIcon(0, QIcon(":/Graphics/Images/StillSmall.png"));
                widget->setText(0, model.getName());
                widget->setText(1, QString("%1").arg(model.getId()));
                widget->setText(2, model.getLabel());
                widget->setText(3, model.getDeviceName());
                widget->setText(4, model.getType());
                widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                widget->setText(6, model.getTimecode());
            }
            else if (model.getType() == "MOVIE")
            {
                QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetVideo);
                widget->setIcon(0, QIcon(":/Graphics/Images/MovieSmall.png"));
                widget->setText(0, model.getName());
                widget->setText(1, QString("%1").arg(model.getId()));
                widget->setText(2, model.getLabel());
                widget->setText(3, model.getDeviceName());
                widget->setText(4, model.getType());
                widget->setText(5, QString("%1").arg(model.getThumbnailId()));
                widget->setText(6, model.getTimecode());
            }
        }
    }

    this->toolBoxLibrary->setItemText(Library::AUDIO_PAGE_INDEX, QString("Audio (%1)").arg(this->treeWidgetAudio->topLevelItemCount()));
    this->toolBoxLibrary->setItemText(Library::STILL_PAGE_INDEX, QString("Images (%1)").arg(this->treeWidgetImage->topLevelItemCount()));
    this->toolBoxLibrary->setItemText(Library::MOVIE_PAGE_INDEX, QString("Videos (%1)").arg(this->treeWidgetVideo->topLevelItemCount()));
}

void LibraryWidget::templateChanged(const TemplateChangedEvent& event)
{
    // TODO: Only add / remove necessary items.
    this->treeWidgetTemplate->clear();
    this->treeWidgetTemplate->clearSelection();

    QList<LibraryModel> models;
    if (this->lineEditFilter->text().isEmpty() &&  dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter().count() == 0)
        models = DatabaseManager::getInstance().getLibraryTemplate();
    else
        models = DatabaseManager::getInstance().getLibraryTemplateByFilter(this->lineEditFilter->text(), dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter());

    if (models.count() > 0)
    {
        foreach (LibraryModel model, models)
        {
            QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetTemplate);
            widget->setIcon(0, QIcon(":/Graphics/Images/TemplateSmall.png"));
            widget->setText(0, model.getName());
            widget->setText(1, QString("%1").arg(model.getId()));
            widget->setText(2, model.getLabel());
            widget->setText(3, model.getDeviceName());
            widget->setText(4, model.getType());
            widget->setText(5, QString("%1").arg(model.getThumbnailId()));
            widget->setText(6, model.getTimecode());
        }
    }

    this->toolBoxLibrary->setItemText(Library::TEMPLATE_PAGE_INDEX, QString("Templates (%1)").arg(this->treeWidgetTemplate->topLevelItemCount()));
}

void LibraryWidget::dataChanged(const DataChangedEvent& event)
{
    // TODO: Only add / remove necessary items.
    this->treeWidgetData->clear();
    this->treeWidgetData->clearSelection();

    QList<LibraryModel> models;
    if (this->lineEditFilter->text().isEmpty() &&  dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter().count() == 0)
        models = DatabaseManager::getInstance().getLibraryData();
    else
        models = DatabaseManager::getInstance().getLibraryDataByFilter(this->lineEditFilter->text(), dynamic_cast<DeviceFilterWidget*>(this->widgetDeviceFilter)->getDeviceFilter());

    if (models.count() > 0)
    {
        foreach (LibraryModel model, models)
        {
            QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetData);
            widget->setIcon(0, QIcon(":/Graphics/Images/DataSmall.png"));
            widget->setText(0, model.getName());
            widget->setText(1, QString("%1").arg(model.getId()));
            widget->setText(2, model.getLabel());
            widget->setText(3, model.getDeviceName());
            widget->setText(4, model.getType());
            widget->setText(5, QString("%1").arg(model.getThumbnailId()));
            widget->setText(6, model.getTimecode());
        }
    }

    this->toolBoxLibrary->setItemText(Library::DATA_PAGE_INDEX, QString("Stored Data (%1)").arg(this->treeWidgetData->topLevelItemCount()));
}

void LibraryWidget::presetChanged(const PresetChangedEvent& event)
{
    // TODO: Only add / remove necessary items.
    this->treeWidgetPreset->clear();
    this->treeWidgetPreset->clearSelection();

    EventManager::getInstance().fireExportPresetMenuEvent(ExportPresetMenuEvent(false));

    QList<PresetModel> models;
    if (this->lineEditFilter->text().isEmpty())
        models = DatabaseManager::getInstance().getPreset();
    else
        models = DatabaseManager::getInstance().getPresetByFilter(this->lineEditFilter->text());

    if (models.count() > 0)
    {
        foreach (PresetModel model, models)
        {
            QTreeWidgetItem* widget = new QTreeWidgetItem(this->treeWidgetPreset);
            widget->setIcon(0, QIcon(":/Graphics/Images/PresetSmall.png"));
            widget->setText(0, model.getName());
            widget->setText(1, QString("%1").arg(model.getId()));
            widget->setText(2, model.getValue());
        }
    }

    this->toolBoxLibrary->setItemText(Library::PRESET_PAGE_INDEX, QString("Presets (%1)").arg(this->treeWidgetPreset->topLevelItemCount()));
}

void LibraryWidget::importPreset(const ImportPresetEvent& event)
{
    QString path = QFileDialog::getOpenFileName(this, "Import Preset", "", "Preset (*.xml)");
    if (!path.isEmpty())
    {
        QFile file(path);
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            const QString& data = file.readAll();
            if (!data.isEmpty())
            {
                QFileInfo info(path);
                DatabaseManager::getInstance().insertPreset(PresetModel(0, info.baseName(), data));

                EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
            }

            file.close();
        }
    }
}

void LibraryWidget::exportPreset(const ExportPresetEvent& event)
{
    if (this->treeWidgetPreset->selectedItems().count() == 0)
        return;

    const QString path = QFileDialog::getSaveFileName(this, "Export Preset", this->treeWidgetPreset->currentItem()->text(0), "Preset (*.xml)");
    if (!path.isEmpty())
    {
        QFile file(path);
        if (file.exists())
            file.remove();

        if (file.open(QFile::WriteOnly))
        {
            const QString& data = this->treeWidgetPreset->currentItem()->text(2);
            if (!data.isEmpty())
            {
                QTextStream stream(&file);
                stream << data;
            }

            file.close();
        }
    }
}

void LibraryWidget::loadLibrary()
{
    EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
    EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
    EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
    EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
}


void LibraryWidget::customContextMenuRequested(const QPoint& point)
{
    if (this->toolBoxLibrary->currentIndex() == Library::TOOLS_PAGE_INDEX)
    {
        if (this->treeWidgetTool->selectedItems().count() == 0)
            return;

        foreach (QTreeWidgetItem* item, this->treeWidgetTool->selectedItems())
        {
            if (item->parent() == NULL) // Top level item.
                return;
        }

        this->contextMenu->exec(this->treeWidgetTool->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
    {
        if (this->treeWidgetAudio->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetAudio->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::STILL_PAGE_INDEX)
    {
        if (this->treeWidgetImage->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetImage->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
    {
        if (this->treeWidgetTemplate->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetTemplate->mapToGlobal(point));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
    {
        if (this->treeWidgetVideo->selectedItems().count() == 0)
            return;

        this->contextMenu->exec(this->treeWidgetVideo->mapToGlobal(point));
    }
}

void LibraryWidget::customContextMenuImageRequested(const QPoint& point)
{
    if (this->treeWidgetImage->selectedItems().count() == 0)
        return;

    this->contextMenuImage->exec(this->treeWidgetImage->mapToGlobal(point));
}

void LibraryWidget::customContextMenuPresetRequested(const QPoint& point)
{
    if (this->treeWidgetPreset->selectedItems().count() == 0)
        return;

    this->contextMenuPreset->exec(this->treeWidgetPreset->mapToGlobal(point));
}

void LibraryWidget::customContextMenuDataRequested(const QPoint& point)
{
    if (this->treeWidgetData->selectedItems().count() == 0)
        return;

    this->contextMenuData->exec(this->treeWidgetData->mapToGlobal(point));
}

void LibraryWidget::contextMenuTriggered(QAction* action)
{
    if (this->toolBoxLibrary->currentIndex() == Library::TOOLS_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetTool->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt(),
                                                                             item->text(6)));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetAudio->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt(),
                                                                             item->text(6)));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetTemplate->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt(),
                                                                             item->text(6)));
    }
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetVideo->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt(),
                                                                             item->text(6)));
    }
}

void LibraryWidget::contextMenuImageTriggered(QAction* action)
{
    if (action->text() == "Add image")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetImage->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), item->text(4), item->text(5).toInt(),
                                                                             item->text(6)));
    }
    else if (action->text() == "Add as image scroller")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetImage->selectedItems())
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(item->text(1).toInt(), item->text(2), item->text(0),
                                                                             item->text(3), "IMAGESCROLLER", item->text(5).toInt(),
                                                                             item->text(6)));
    }
}

void LibraryWidget::contextMenuDataTriggered(QAction* action)
{
    if (action->text() == "Add stored data")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetData->selectedItems())
            EventManager::getInstance().fireAddTemplateDataEvent(AddTemplateDataEvent(item->text(0), true));
    }
}

void LibraryWidget::contextMenuPresetTriggered(QAction* action)
{
    if (action->text() == "Add item")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetPreset->selectedItems())
            EventManager::getInstance().fireAddPresetItemEvent(AddPresetItemEvent(item->text(2)));
    }
    else if (action->text() == "Delete")
        this->treeWidgetPreset->removeSelectedPresets();
}

void LibraryWidget::filterLibrary()
{
    EventManager::getInstance().fireLibraryFilterChangedEvent(LibraryFilterChangedEvent(this->lineEditFilter->text()));

    EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
    EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
    EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
    EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
}

void LibraryWidget::itemDoubleClicked(QTreeWidgetItem* current, int index)
{
    if (current == NULL)
        return;

    if (this->toolBoxLibrary->currentIndex() == Library::TOOLS_PAGE_INDEX)
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt(),
                                                                         current->text(6)));
    else if (this->toolBoxLibrary->currentIndex() == Library::AUDIO_PAGE_INDEX)
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt(),
                                                                         current->text(6)));
    else if (this->toolBoxLibrary->currentIndex() == Library::STILL_PAGE_INDEX)
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt(),
                                                                         current->text(6)));
    else if (this->toolBoxLibrary->currentIndex() == Library::TEMPLATE_PAGE_INDEX)
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt(),
                                                                         current->text(6)));
    else if (this->toolBoxLibrary->currentIndex() == Library::MOVIE_PAGE_INDEX)
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                         current->text(3), current->text(4), current->text(5).toInt(),
                                                                         current->text(6)));
    else if (this->toolBoxLibrary->currentIndex() == Library::DATA_PAGE_INDEX)
        EventManager::getInstance().fireAddTemplateDataEvent(AddTemplateDataEvent(current->text(0), true));
    else if (this->toolBoxLibrary->currentIndex() == Library::PRESET_PAGE_INDEX)
        EventManager::getInstance().fireAddPresetItemEvent(AddPresetItemEvent(current->text(2)));
}

void LibraryWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    if (this->toolBoxLibrary->currentIndex() == Library::PRESET_PAGE_INDEX)
    {
        EventManager::getInstance().fireExportPresetMenuEvent(ExportPresetMenuEvent(true));
        return;
    }

    this->model = QSharedPointer<LibraryModel>(new LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                current->text(3), current->text(4), current->text(5).toInt(),
                                                                current->text(6)));

    if (this->toolBoxLibrary->currentIndex() == Library::DATA_PAGE_INDEX)
        return;

    EventManager::getInstance().fireLibraryItemSelectedEvent(LibraryItemSelectedEvent(NULL, this->model.data()));
}

void LibraryWidget::toggleExpandItem(QTreeWidgetItem* item, int index)
{
    if (item->parent() != NULL) // The item is not a top level item.
        return;

    item->setExpanded(!item->isExpanded());

    this->treeWidgetTool->clearSelection();
}
