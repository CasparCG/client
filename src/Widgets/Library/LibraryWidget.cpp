#include "LibraryWidget.h"

#include "Global.h"

#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "DeviceFilterWidget.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Models/LibraryModel.h"
#include "Models/PresetModel.h"

#include <QtCore/QPoint>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QTextStream>
#include <QtCore/QModelIndex>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStandardItemModel>

#include <QtCore/QTimer>

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

    qApp->installEventFilter(this);

    QTimer::singleShot(1, this, SLOT(loadLibrary()));
}

void LibraryWidget::loadLibrary()
{
    EventManager::getInstance().fireMediaChangedEvent();
    EventManager::getInstance().fireTemplateChangedEvent();
    EventManager::getInstance().fireDataChangedEvent();
    EventManager::getInstance().firePresetChangedEvent();
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

    QTreeWidgetItem* widgetImage = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetImage->setIcon(0, QIcon(":/Graphics/Images/StillSmall.png"));
    widgetImage->setText(0, "Image");
    widgetImage->setText(1, "0");
    widgetImage->setText(2, "Image");
    widgetImage->setText(3, "");
    widgetImage->setText(4, Rundown::IMAGE);
    widgetImage->setText(5, "0");
    widgetImage->setText(6, "");

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

    QTreeWidgetItem* widgetVideo = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(0));
    widgetVideo->setIcon(0, QIcon(":/Graphics/Images/MovieSmall.png"));
    widgetVideo->setText(0, "Video");
    widgetVideo->setText(1, "0");
    widgetVideo->setText(2, "Video");
    widgetVideo->setText(3, "");
    widgetVideo->setText(4, Rundown::VIDEO);
    widgetVideo->setText(5, "0");
    widgetVideo->setText(6, "");

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

    QTreeWidgetItem* widgetGeometry = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetGeometry->setIcon(0, QIcon(":/Graphics/Images/GeometrySmall.png"));
    widgetGeometry->setText(0, "Transformation");
    widgetGeometry->setText(1, "0");
    widgetGeometry->setText(2, "Transformation");
    widgetGeometry->setText(3, "");
    widgetGeometry->setText(4, Rundown::GEOMETRY);
    widgetGeometry->setText(5, "0");

    QTreeWidgetItem* widgetGrid = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetGrid->setIcon(0, QIcon(":/Graphics/Images/GridSmall.png"));
    widgetGrid->setText(0, "Grid");
    widgetGrid->setText(1, "0");
    widgetGrid->setText(2, "Grid");
    widgetGrid->setText(3, "");
    widgetGrid->setText(4, Rundown::GRID);
    widgetGrid->setText(5, "0");

    QTreeWidgetItem* widgetKeyer = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetKeyer->setIcon(0, QIcon(":/Graphics/Images/KeyerSmall.png"));
    widgetKeyer->setText(0, "Mask");
    widgetKeyer->setText(1, "0");
    widgetKeyer->setText(2, "Mask");
    widgetKeyer->setText(3, "");
    widgetKeyer->setText(4, Rundown::KEYER);
    widgetKeyer->setText(5, "0");

    QTreeWidgetItem* widgetLevels = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetLevels->setIcon(0, QIcon(":/Graphics/Images/LevelsSmall.png"));
    widgetLevels->setText(0, "Levels");
    widgetLevels->setText(1, "0");
    widgetLevels->setText(2, "Levels");
    widgetLevels->setText(3, "");
    widgetLevels->setText(4, Rundown::LEVELS);
    widgetLevels->setText(5, "0");

    QTreeWidgetItem* widgetOpacity = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetOpacity->setIcon(0, QIcon(":/Graphics/Images/OpacitySmall.png"));
    widgetOpacity->setText(0, "Opacity");
    widgetOpacity->setText(1, "0");
    widgetOpacity->setText(2, "Opacity");
    widgetOpacity->setText(3, "");
    widgetOpacity->setText(4, Rundown::OPACITY);
    widgetOpacity->setText(5, "0");

    QTreeWidgetItem* widgetSaturation = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetSaturation->setIcon(0, QIcon(":/Graphics/Images/SaturationSmall.png"));
    widgetSaturation->setText(0, "Saturation");
    widgetSaturation->setText(1, "0");
    widgetSaturation->setText(2, "Saturation");
    widgetSaturation->setText(3, "");
    widgetSaturation->setText(4, Rundown::SATURATION);
    widgetSaturation->setText(5, "0");

    QTreeWidgetItem* widgetVolume = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetVolume->setIcon(0, QIcon(":/Graphics/Images/VolumeSmall.png"));
    widgetVolume->setText(0, "Volume");
    widgetVolume->setText(1, "0");
    widgetVolume->setText(2, "Volume");
    widgetVolume->setText(3, "");
    widgetVolume->setText(4, Rundown::VOLUME);
    widgetVolume->setText(5, "0");

    QTreeWidgetItem* widgetCommit = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(1));
    widgetCommit->setIcon(0, QIcon(":/Graphics/Images/CommitSmall.png"));
    widgetCommit->setText(0, "Commit");
    widgetCommit->setText(1, "0");
    widgetCommit->setText(2, "Commit");
    widgetCommit->setText(3, "");
    widgetCommit->setText(4, Rundown::COMMIT);
    widgetCommit->setText(5, "0");

    QTreeWidgetItem* widgetSolidColor = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetSolidColor->setIcon(0, QIcon(":/Graphics/Images/SolidColorSmall.png"));
    widgetSolidColor->setText(0, "Solid Color");
    widgetSolidColor->setText(1, "0");
    widgetSolidColor->setText(2, "Solid Color");
    widgetSolidColor->setText(3, "");
    widgetSolidColor->setText(4, Rundown::SOLIDCOLOR);
    widgetSolidColor->setText(5, "0");

    QTreeWidgetItem* widgetGpiOutput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetGpiOutput->setIcon(0, QIcon(":/Graphics/Images/GpiOutputSmall.png"));
    widgetGpiOutput->setText(0, "GPI Output");
    widgetGpiOutput->setText(1, "0");
    widgetGpiOutput->setText(2, "GPI Output");
    widgetGpiOutput->setText(3, "");
    widgetGpiOutput->setText(4, Rundown::GPIOUTPUT);
    widgetGpiOutput->setText(5, "0");

    QTreeWidgetItem* widgetFileRecorder = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetFileRecorder->setIcon(0, QIcon(":/Graphics/Images/FileRecorderSmall.png"));
    widgetFileRecorder->setText(0, "File Recorder");
    widgetFileRecorder->setText(1, "0");
    widgetFileRecorder->setText(2, "File Recorder");
    widgetFileRecorder->setText(3, "");
    widgetFileRecorder->setText(4, Rundown::FILERECORDER);
    widgetFileRecorder->setText(5, "0");

    QTreeWidgetItem* widgetDeckLinkInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetDeckLinkInput->setIcon(0, QIcon(":/Graphics/Images/DeckLinkProducerSmall.png"));
    widgetDeckLinkInput->setText(0, "DeckLink Input");
    widgetDeckLinkInput->setText(1, "0");
    widgetDeckLinkInput->setText(2, "DeckLink Input");
    widgetDeckLinkInput->setText(3, "");
    widgetDeckLinkInput->setText(4, Rundown::DECKLINKINPUT);
    widgetDeckLinkInput->setText(5, "0");

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

    QTreeWidgetItem* widgetChroma = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetChroma->setIcon(0, QIcon(":/Graphics/Images/ChromaSmall.png"));
    widgetChroma->setText(0, "Chroma Key");
    widgetChroma->setText(1, "0");
    widgetChroma->setText(2, "Chroma Key");
    widgetChroma->setText(3, "");
    widgetChroma->setText(4, Rundown::CHROMAKEY);
    widgetChroma->setText(5, "0");

    QTreeWidgetItem* widgetSeparator = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(2));
    widgetSeparator->setIcon(0, QIcon(":/Graphics/Images/SeparatorSmall.png"));
    widgetSeparator->setText(0, "Separator");
    widgetSeparator->setText(1, "0");
    widgetSeparator->setText(2, "Separator");
    widgetSeparator->setText(3, "");
    widgetSeparator->setText(4, Rundown::SEPARATOR);
    widgetSeparator->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterAuto = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterAuto->setIcon(0, QIcon(":/Graphics/Images/MixerSmall.png"));
    widgetTriCasterAuto->setText(0, "Trigger Auto");
    widgetTriCasterAuto->setText(1, "0");
    widgetTriCasterAuto->setText(2, "Trigger Auto");
    widgetTriCasterAuto->setText(3, "");
    widgetTriCasterAuto->setText(4, Rundown::AUTO);
    widgetTriCasterAuto->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterTake = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterTake->setIcon(0, QIcon(":/Graphics/Images/MixerSmall.png"));
    widgetTriCasterTake->setText(0, "Trigger Take");
    widgetTriCasterTake->setText(1, "0");
    widgetTriCasterTake->setText(2, "Trigger Take");
    widgetTriCasterTake->setText(3, "");
    widgetTriCasterTake->setText(4, Rundown::TAKE);
    widgetTriCasterTake->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterProgramInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterProgramInput->setIcon(0, QIcon(":/Graphics/Images/MixerSmall.png"));
    widgetTriCasterProgramInput->setText(0, "Switch Program Input");
    widgetTriCasterProgramInput->setText(1, "0");
    widgetTriCasterProgramInput->setText(2, "Switch Program Input");
    widgetTriCasterProgramInput->setText(3, "");
    widgetTriCasterProgramInput->setText(4, Rundown::PROGRAMINPUT);
    widgetTriCasterProgramInput->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterPreviewInput = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterPreviewInput->setIcon(0, QIcon(":/Graphics/Images/MixerSmall.png"));
    widgetTriCasterPreviewInput->setText(0, "Switch Preview Input");
    widgetTriCasterPreviewInput->setText(1, "0");
    widgetTriCasterPreviewInput->setText(2, "Switch Preview Input");
    widgetTriCasterPreviewInput->setText(3, "");
    widgetTriCasterPreviewInput->setText(4, Rundown::PREVIEWINPUT);
    widgetTriCasterPreviewInput->setText(5, "0");

    QTreeWidgetItem* widgetTriCasterPreset = new QTreeWidgetItem(this->treeWidgetTool->topLevelItem(3));
    widgetTriCasterPreset->setIcon(0, QIcon(":/Graphics/Images/MixerSmall.png"));
    widgetTriCasterPreset->setText(0, "Select Preset");
    widgetTriCasterPreset->setText(1, "0");
    widgetTriCasterPreset->setText(2, "Select Preset");
    widgetTriCasterPreset->setText(3, "");
    widgetTriCasterPreset->setText(4, Rundown::PRESET);
    widgetTriCasterPreset->setText(5, "0");

    this->treeWidgetTool->expandAll();
}

void LibraryWidget::setupUiMenu()
{
    this->contextMenu = new QMenu(this);
    this->contextMenu->addAction("Add item");

    this->contextMenuData = new QMenu(this);
    this->contextMenuData->addAction("Add template data");
    this->contextMenuData->addAction("Add template data as stored data");

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

bool LibraryWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (target == this->treeWidgetPreset)
        {
            if (keyEvent->key() == Qt::Key_Delete)
                return removeSelectedPresets();
        }
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::MediaChanged))
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
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::TemplateChanged))
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
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::DataChanged))
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
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::PresetChanged))
    {
        // TODO: Only add / remove necessary items.
        this->treeWidgetPreset->clear();
        this->treeWidgetPreset->clearSelection();

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
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ImportPreset))
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

                    EventManager::getInstance().firePresetChangedEvent();
                }

                file.close();
            }
        }
    }
    else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ExportPreset))
    {
        if (this->treeWidgetPreset->selectedItems().count() == 0)
            return true;

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

    return QObject::eventFilter(target, event);
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
    if (action->text() == "Add template data")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetData->selectedItems())
            EventManager::getInstance().fireAddTemplateDataEvent(item->text(0), false);
    }
    else if (action->text() == "Add template data as stored data")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetData->selectedItems())
            EventManager::getInstance().fireAddTemplateDataEvent(item->text(0), true);
    }
}

void LibraryWidget::contextMenuPresetTriggered(QAction* action)
{
    if (action->text() == "Add item")
    {
        foreach (QTreeWidgetItem* item, this->treeWidgetPreset->selectedItems())
            EventManager::getInstance().fireAddPresetItemEvent(item->text(2));
    }
    else if (action->text() == "Delete")
        removeSelectedPresets();
}

bool LibraryWidget::removeSelectedPresets()
{
    foreach (QTreeWidgetItem* item, this->treeWidgetPreset->selectedItems())
        DatabaseManager::getInstance().deletePreset(item->text(1).toInt());

    EventManager::getInstance().firePresetChangedEvent();

    return true;
}

void LibraryWidget::filterLibrary()
{
    EventManager::getInstance().fireMediaChangedEvent();
    EventManager::getInstance().fireTemplateChangedEvent();
    EventManager::getInstance().fireDataChangedEvent();
    EventManager::getInstance().firePresetChangedEvent();
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
    else if (this->toolBoxLibrary->currentIndex() == Library::PRESET_PAGE_INDEX)
        EventManager::getInstance().fireAddPresetItemEvent(current->text(2));
}

void LibraryWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (current == NULL)
        return;

    if (this->toolBoxLibrary->currentIndex() == Library::PRESET_PAGE_INDEX)
        return;

    this->model = QSharedPointer<LibraryModel>(new LibraryModel(current->text(1).toInt(), current->text(2), current->text(0),
                                                                current->text(3), current->text(4), current->text(5).toInt(),
                                                                current->text(6)));

    if (this->toolBoxLibrary->currentIndex() == Library::DATA_PAGE_INDEX)
        return;

    EventManager::getInstance().fireLibraryItemSelectedEvent(NULL, this->model.data());
}

void LibraryWidget::toggleExpandItem(QTreeWidgetItem* item, int index)
{
    if (item->parent() != NULL) // The item is not a top level item.
        return;

    item->setExpanded(!item->isExpanded());

    this->treeWidgetTool->clearSelection();
}
