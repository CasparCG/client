#include "RundownTreeWidget.h"
#include "RundownBlendModeWidget.h"
#include "RundownBrightnessWidget.h"
#include "RundownCommitWidget.h"
#include "RundownContrastWidget.h"
#include "RundownClipWidget.h"
#include "RundownDeckLinkInputWidget.h"
#include "RundownFileRecorderWidget.h"
#include "RundownImageScrollerWidget.h"
#include "RundownFillWidget.h"
#include "RundownGpiOutputWidget.h"
#include "RundownGridWidget.h"
#include "RundownGroupWidget.h"
#include "RundownKeyerWidget.h"
#include "RundownLevelsWidget.h"
#include "RundownMovieWidget.h"
#include "RundownOpacityWidget.h"
#include "RundownSaturationWidget.h"
#include "RundownTemplateWidget.h"
#include "RundownVolumeWidget.h"
#include "RundownSeparatorWidget.h"
#include "RundownPrintWidget.h"
#include "RundownClearOutputWidget.h"
#include "RundownSolidColorWidget.h"
#include "RundownAudioWidget.h"
#include "RundownStillWidget.h"
#include "RundownItemFactory.h"
#include "PresetDialog.h"

#include "GpiManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "DeviceManager.h"
#include "Events/PresetChangedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/SaveMenuEvent.h"
#include "Events/Rundown/SaveAsMenuEvent.h"
#include "Events/Rundown/ReloadRundownMenuEvent.h"
#include "Models/RundownModel.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QPoint>
#include <QtCore5Compat/QTextCodec>
#include <QtCore/QElapsedTimer>
#include <QtCore/QTextStream>
#include <QtCore/QCryptographicHash>

#include <QtGui/QClipboard>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>

#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidgetItem>

RundownTreeWidget::RundownTreeWidget(QWidget* parent)
    : QWidget(parent),
      active(false), enterPressed(false), allowRemoteRundownTriggering(false), repositoryRundown(false), previewOnAutoStep(false),
      clearDelayedCommandsOnAutoStep(false), activeRundown(Rundown::DEFAULT_NAME), currentAutoPlayWidget(NULL), copyItem(NULL), currentPlayingItem(NULL), currentPlayingAutoStepItem(NULL),
      upControlSubscription(NULL), downControlSubscription(NULL), playAndAutoStepControlSubscription(NULL), playNowAndAutoStepControlSubscription(NULL),
      playNowIfChannelControlSubscription(NULL), stopControlSubscription(NULL), playControlSubscription(NULL), playNowControlSubscription(NULL),
      loadControlSubscription(NULL), pauseControlSubscription(NULL), nextControlSubscription(NULL), updateControlSubscription(NULL), invokeControlSubscription(NULL), previewControlSubscription(NULL),
      clearControlSubscription(NULL), clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL), repositoryDevice(NULL)
{
    setupUi(this);
    setupMenus();

    this->previewOnAutoStep = (DatabaseManager::getInstance().getConfigurationByName("PreviewOnAutoStep").getValue() == "true") ? true : false;
    this->clearDelayedCommandsOnAutoStep = (DatabaseManager::getInstance().getConfigurationByName("ClearDelayedCommandsOnAutoStep").getValue() == "true") ? true : false;

    QObject::connect(this->treeWidgetRundown, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    // TODO: Specific Gpi device.
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(gpiTriggered(int, GpiDevice*)), this, SLOT(gpiPortTriggered(int, GpiDevice*)));

    QObject::connect(&EventManager::getInstance(), SIGNAL(clearDelayedCommands()), this, SLOT(clearDelayedCommands()));
    QObject::connect(&EventManager::getInstance(), SIGNAL(saveAsPreset(const SaveAsPresetEvent&)), this, SLOT(saveAsPreset(const SaveAsPresetEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(addPresetItem(const AddPresetItemEvent&)), this, SLOT(addPresetItem(const AddPresetItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(addRudnownItem(const AddRudnownItemEvent&)), this, SLOT(addRudnownItem(const AddRudnownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(compactView(const CompactViewEvent&)), this, SLOT(compactView(const CompactViewEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executeRundownItem(const ExecuteRundownItemEvent&)), this, SLOT(executeRundownItem(const ExecuteRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(allowRemoteTriggering(const AllowRemoteTriggeringEvent&)), this, SLOT(allowRemoteTriggering(const AllowRemoteTriggeringEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayRundownItem(const AutoPlayRundownItemEvent&)), this, SLOT(autoPlayRundownItem(const AutoPlayRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayChanged(const AutoPlayChangedEvent&)), this, SLOT(autoPlayChanged(const AutoPlayChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&)), this, SLOT(autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executePlayoutCommand(const ExecutePlayoutCommandEvent&)), this, SLOT(executePlayoutCommand(const ExecutePlayoutCommandEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&)), this, SLOT(removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(copyItemProperties(const CopyItemPropertiesEvent&)), this, SLOT(copyItemProperties(const CopyItemPropertiesEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(pasteItemProperties(const PasteItemPropertiesEvent&)), this, SLOT(pasteItemProperties(const PasteItemPropertiesEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(insertRepositoryChanges(const InsertRepositoryChangesEvent&)), this, SLOT(insertRepositoryChanges(const InsertRepositoryChangesEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(clearCurrentPlayingItem(const ClearCurrentPlayingItemEvent&)), this, SLOT(clearCurrentPlayingItem(const ClearCurrentPlayingItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(currentItemChanged(const CurrentItemChangedEvent&)), this, SLOT(currentItemChanged(const CurrentItemChangedEvent&)));

    foreach (const GpiPortModel& port, DatabaseManager::getInstance().getGpiPorts())
        gpiBindingChanged(port.getPort(), port.getAction());

    this->treeWidgetRundown->checkEmptyRundown();
}

void RundownTreeWidget::setupMenus()
{
    this->contextMenuMixer = new QMenu(this);
    this->contextMenuMixer->setObjectName("contextMenuMixer");
    this->contextMenuMixer->setTitle("Mixer");
    //this->contextMenuMixer->setIcon(QIcon(":/Graphics/Images/Mixer.png"));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/AnchorSmall.png"), "Anchor Point", this, SLOT(addAnchorItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BlendModeSmall.png"), "Blend Mode", this, SLOT(addBlendModeItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BrightnessSmall.png"), "Brightness", this, SLOT(addBrightnessItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ChromaSmall.png"), "Chroma Key", this, SLOT(addChromaKeyItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ClipSmall.png"), "Clipping", this, SLOT(addClipItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CommitSmall.png"), "Commit", this, SLOT(addCommitItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ContrastSmall.png"), "Contrast", this, SLOT(addContrastItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CropSmall.png"), "Crop", this, SLOT(addCropItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/PerspectiveSmall.png"), "Distort", this, SLOT(addPerspectiveItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/GridSmall.png"), "Grid", this, SLOT(addGridItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/LevelsSmall.png"), "Levels", this, SLOT(addLevelsItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/KeyerSmall.png"), "Mask", this, SLOT(addKeyerItem())); 
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/OpacitySmall.png"), "Opacity", this, SLOT(addOpacityItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ResetSmall.png"), "Reset", this, SLOT(addResetItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/RotationSmall.png"), "Rotation", this, SLOT(addRotationItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/SaturationSmall.png"), "Saturation", this, SLOT(addSaturationItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/FillSmall.png"), "Transform", this, SLOT(addFillItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/VolumeSmall.png"), "Volume", this, SLOT(addVolumeItem()));

    this->contextMenuLibrary = new QMenu(this);
    this->contextMenuLibrary->setObjectName("contextMenuLibrary");
    this->contextMenuLibrary->setTitle("Library");
    //this->contextMenuLibrary->setIcon(QIcon(":/Graphics/Images/Library.png"));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/AudioSmall.png"), "Audio", this, SLOT(addAudioItem()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/StillSmall.png"), "Image", this, SLOT(addImageItem()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/ImageScrollerSmall.png"), "Image Scroller", this, SLOT(addImageScrollerItem()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/TemplateSmall.png"), "Template", this, SLOT(addTemplateItem()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/MovieSmall.png"), "Video", this, SLOT(addVideoItem()));

    this->contextMenuOther = new QMenu(this);
    this->contextMenuOther->setObjectName("contextMenuOther");
    this->contextMenuOther->setTitle("Other");
    //this->contextMenuOther->setIcon(QIcon(":/Graphics/Images/Other.png"));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SnapshotSmall.png"), "Channel Snapshot", this, SLOT(addPrintItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/ClearSmall.png"), "Clear Output", this, SLOT(addClearOutputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/CustomCommandSmall.png"), "Custom Command", this, SLOT(addCustomCommandItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/DeckLinkProducerSmall.png"), "DeckLink Input", this, SLOT(addDeckLinkInputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SolidColorSmall.png"), "Fade Out", this, SLOT(addFadeToBlackItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/FileRecorderSmall.png"), "File Recorder", this, SLOT(addFileRecorderItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/GpiOutputSmall.png"), "GPI Output", this, SLOT(addGpiOutputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/HtmlSmall.png"), "HTML Page", this, SLOT(addHtmlItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/HttpGetSmall.png"), "HTTP GET Request", this, SLOT(addHttpGetItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/HttpGetSmall.png"), "HTTP POST Request", this, SLOT(addHttpPostItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/OscOutputSmall.png"), "OSC Output", this, SLOT(addOscOutputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/PlayoutCommandSmall.png"), "Playout Command", this, SLOT(addPlayoutCommandItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/RouteChannelSmall.png"), "Route Channel", this, SLOT(addRouteChannelItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/RouteVideolayerSmall.png"), "Route Video Layer", this, SLOT(addRouteVideolayerItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SeparatorSmall.png"), "Separator", this, SLOT(addSeparatorItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SolidColorSmall.png"), "Solid Color", this, SLOT(addSolidColorItem()));

    this->contextMenuTools = new QMenu(this);
    this->contextMenuTools->setTitle("Tools");
    //this->contextMenuTools->setIcon(QIcon(":/Graphics/Images/New.png"));
    this->contextMenuTools->addMenu(this->contextMenuLibrary);
    this->contextMenuTools->addMenu(this->contextMenuMixer);
    this->contextMenuTools->addMenu(this->contextMenuOther);

    this->contextMenuMark = new QMenu(this);
    this->contextMenuMark->setTitle("Mark Item");
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "As Used", this, SLOT(markItemAsUsed()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "As Unused", this, SLOT(markItemAsUnused()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "All as Used", this, SLOT(markAllItemsAsUsed()));
    this->contextMenuMark->addAction(/*QIcon(":/Graphics/Images/RenameRundown.png"),*/ "All as Unused", this, SLOT(markAllItemsAsUnused()));

    this->contextMenuColor = new QMenu(this);
    this->contextMenuColor->setTitle("Colorize Item");
    //this->contextMenuColor->setIcon(QIcon(":/Graphics/Images/Color.png"));
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Chocolate.png"),*/ "Chocolate");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/DarkKhaki.png"),*/ "DarkKhaki");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/DarkSlateGray.png"),*/ "DarkSlateGray");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Maroon.png"),*/ "Maroon");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/MaroonLight.png"),*/ "MaroonLight");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/OliveDrab.png"),*/ "OliveDrab");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/RoyalBlue.png"),*/ "RoyalBlue");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/SeaGreen.png"),*/ "SeaGreen");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Sienna.png"),*/ "Sienna");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/SteelBlue.png"),*/ "SteelBlue");
    this->contextMenuColor->addSeparator();
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Reset");

    foreach (QAction* action, this->contextMenuColor->actions())
    {
        action->setCheckable(true);
        action->setChecked(false);
        action->font().setItalic(true);
    }

    this->contextMenuRundown = new QMenu(this);
    this->contextMenuRundown->addMenu(this->contextMenuTools);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/GroupSmall.png"),*/ "Group");
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/UngroupSmall.png"),*/ "Ungroup");
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addMenu(this->contextMenuMark);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction("Copy Properties", this, SLOT(copyItemProperties()));
    this->contextMenuRundown->addAction("Paste Properties", this, SLOT(pasteItemProperties()));
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addMenu(this->contextMenuColor);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/PresetSmall.png"),*/ "Save as Preset...", this, SLOT(saveAsPreset()));
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/Remove.png"),*/ "Remove", this, SLOT(removeSelectedItems()));

    QObject::connect(this->contextMenuTools, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuNewTriggered(QAction*)));
    QObject::connect(this->contextMenuColor, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuColorTriggered(QAction*)));
    QObject::connect(this->contextMenuRundown, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuRundownTriggered(QAction*)));
}

void RundownTreeWidget::executePlayoutCommand(const ExecutePlayoutCommandEvent& event)
{
    if (!this->active)
        return;

    if (event.getKey() == Qt::Key_F1) // Stop.
        executeCommand(Playout::PlayoutType::Stop, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F2 && event.getModifiers() == Qt::ShiftModifier) // Play Now.
        executeCommand(Playout::PlayoutType::PlayNow, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F2) // Play.
        executeCommand(Playout::PlayoutType::Play, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F3) // Load.
        executeCommand(Playout::PlayoutType::Load, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F4) // Pause.
        executeCommand(Playout::PlayoutType::PauseResume, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F5) // Next.
        executeCommand(Playout::PlayoutType::Next, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F6) // Update.
        executeCommand(Playout::PlayoutType::Update, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F7) // Invoke.
        executeCommand(Playout::PlayoutType::Invoke, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F8) // Preview.
        executeCommand(Playout::PlayoutType::Preview, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F10) // Clear.
        executeCommand(Playout::PlayoutType::Clear, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F11) // Clear videolayer.
        executeCommand(Playout::PlayoutType::ClearVideoLayer, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F12) // Clear channel.
        executeCommand(Playout::PlayoutType::ClearChannel, Action::ActionType::KeyPress);
}

void RundownTreeWidget::saveAsPreset(const SaveAsPresetEvent& event)
{
    Q_UNUSED(event);

    if (!this->active)
        return;

    saveAsPreset();
}

void RundownTreeWidget::copyItemProperties()
{
    EventManager::getInstance().fireCopyItemPropertiesEvent(CopyItemPropertiesEvent());
}

void RundownTreeWidget::pasteItemProperties()
{
    EventManager::getInstance().firePasteItemPropertiesEvent(PasteItemPropertiesEvent());
}

void RundownTreeWidget::copyItemProperties(const CopyItemPropertiesEvent& event)
{
    Q_UNUSED(event);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    this->treeWidgetRundown->copyItemProperties();
}

void RundownTreeWidget::pasteItemProperties(const PasteItemPropertiesEvent& event)
{
    Q_UNUSED(event);

    if (!this->active)
        return;

    this->treeWidgetRundown->pasteItemProperties();
}

bool RundownTreeWidget::removeSelectedItems()
{
    this->treeWidgetRundown->removeSelectedItems();

    return true;
}

void RundownTreeWidget::markItemAsUsed()
{
    EventManager::getInstance().fireMarkItemAsUsedEvent(MarkItemAsUsedEvent());
}

void RundownTreeWidget::markItemAsUnused()
{
    EventManager::getInstance().fireMarkItemAsUnusedEvent(MarkItemAsUnusedEvent());
}

void RundownTreeWidget::markAllItemsAsUsed()
{
    EventManager::getInstance().fireMarkAllItemsAsUsedEvent(MarkAllItemsAsUsedEvent());
}

void RundownTreeWidget::markAllItemsAsUnused()
{
    EventManager::getInstance().fireMarkAllItemsAsUnusedEvent(MarkAllItemsAsUnusedEvent());
}

void RundownTreeWidget::addPresetItem(const AddPresetItemEvent& event)
{
    if (!this->active)
        return;

    qApp->clipboard()->setText(event.getPreset());
    pasteSelectedItems();
    selectItemBelow();
}

void RundownTreeWidget::compactView(const CompactViewEvent& event)
{
    Q_UNUSED(event);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
        return;

    for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
    {
        QTreeWidgetItem* item = this->treeWidgetRundown->invisibleRootItem()->child(i);
        QWidget* widget = dynamic_cast<QWidget*>(this->treeWidgetRundown->itemWidget(item, 0));

        dynamic_cast<AbstractRundownWidget*>(widget)->setCompactView(!this->treeWidgetRundown->getCompactView());
        if (this->treeWidgetRundown->getCompactView())
            widget->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);
        else
            widget->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);

        for (int j = 0; j < item->childCount(); j++)
        {
            QTreeWidgetItem* child = item->child(j);
            QWidget* widget = dynamic_cast<QWidget*>(this->treeWidgetRundown->itemWidget(child, 0));

            dynamic_cast<AbstractRundownWidget*>(widget)->setCompactView(!this->treeWidgetRundown->getCompactView());
            if (this->treeWidgetRundown->getCompactView())
                widget->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);
            else
                widget->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);
        }
    }

    this->treeWidgetRundown->doItemsLayout(); // Refresh

    this->treeWidgetRundown->setCompactView(!this->treeWidgetRundown->getCompactView());
}

void RundownTreeWidget::executeRundownItem(const ExecuteRundownItemEvent& event)
{
    if (event.getItem()->treeWidget() == this->treeWidgetRundown)
        executeCommand(event.getType(), Action::ActionType::KeyPress, event.getItem());
}

void RundownTreeWidget::clearDelayedCommands()
{
    if (!this->active)
        return;

    if (this->currentPlayingAutoStepItem != nullptr)
    {
        for (int i = 0; i < this->currentPlayingAutoStepItem->childCount(); i++)
        {
            QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->currentPlayingAutoStepItem->child(i), 0);
            dynamic_cast<AbstractRundownWidget*>(childWidget)->clearDelayedCommands();
        }
    }
}

void RundownTreeWidget::allowRemoteTriggering(const AllowRemoteTriggeringEvent& event)
{
    if (!this->active)
        return;

    this->allowRemoteRundownTriggering = event.getEnabled();

    (this->allowRemoteRundownTriggering == true) ? configureOscSubscriptions() : resetOscSubscriptions();
}

void RundownTreeWidget::addRudnownItem(const AddRudnownItemEvent& event)
{
    if (!this->active)
        return;

    AbstractRundownWidget* widget = RundownItemFactory::getInstance().createWidget(event.getLibraryModel());
    if (widget == NULL)
        return;

    widget->setCompactView(this->treeWidgetRundown->getCompactView());

    QTreeWidgetItem* item = new QTreeWidgetItem();
    if (this->treeWidgetRundown->currentItem() == NULL) // There is no item selected.
        this->treeWidgetRundown->invisibleRootItem()->addChild(item); // Add item to the bottom of the rundown.
    else if (this->treeWidgetRundown->currentItem()->parent() == NULL) // Top level item.
        this->treeWidgetRundown->invisibleRootItem()->insertChild(this->treeWidgetRundown->currentIndex().row() + 1, item); // Insert item below.
    else if (this->treeWidgetRundown->currentItem()->parent() != NULL) // Goup item.
    {
        this->treeWidgetRundown->currentItem()->parent()->insertChild(this->treeWidgetRundown->currentIndex().row() + 1, item); // Insert item below.
        widget->setInGroup(true);
    }

    this->treeWidgetRundown->setItemWidget(item, 0, dynamic_cast<QWidget*>(widget));
    this->treeWidgetRundown->setCurrentItem(item);
    this->treeWidgetRundown->setFocus();

    if (this->treeWidgetRundown->getCompactView())
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);

    this->treeWidgetRundown->doItemsLayout(); // Refresh.
    this->treeWidgetRundown->repaint();

    this->treeWidgetRundown->checkEmptyRundown();
}

void RundownTreeWidget::autoPlayChanged(const AutoPlayChangedEvent& event)
{
    if (!this->active)
        return;

    for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
    {
        QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem()->child(i), 0);
        AbstractRundownWidget* childRundownWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);

        if (dynamic_cast<MovieCommand*>(childRundownWidget->getCommand()))
            dynamic_cast<MovieCommand*>(childRundownWidget->getCommand())->setAutoPlay(event.getAutoPlay());
    }
}

void RundownTreeWidget::autoPlayRundownItem(const AutoPlayRundownItemEvent& event)
{
    if (!this->active)
        return;

    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(event.getSource());
    foreach (QList<AbstractRundownWidget*>* autoPlayQueue, this->autoPlayQueues)
    {
        if (autoPlayQueue->contains(rundownWidget))
        {
            // Remove currently playing item.
            autoPlayQueue->removeAt(0);

            // Have more in queue, play them...
            if (!autoPlayQueue->isEmpty())
            {
                AbstractRundownWidget* rundownQueueWidget = dynamic_cast<AbstractRundownWidget*>(autoPlayQueue->at(0));
                dynamic_cast<AbstractPlayoutCommand*>(rundownQueueWidget)->executeCommand(Playout::PlayoutType::Play);

                this->currentAutoPlayWidget = rundownQueueWidget;
            }
            else
                this->autoPlayQueues.removeOne(autoPlayQueue);

            break;
        }
    }
}

void RundownTreeWidget::autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent& event)
{
    if (!this->active)
        return;

    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(event.getSource());

    foreach (QList<AbstractRundownWidget*>* autoPlayQueue, this->autoPlayQueues)
    {
        if (autoPlayQueue->contains(rundownWidget))
        {
            // Have more in queue, play them...
            if (!autoPlayQueue->isEmpty())
            {
                AbstractRundownWidget* rundownQueueWidget = dynamic_cast<AbstractRundownWidget*>(autoPlayQueue->at(0));
                dynamic_cast<AbstractPlayoutCommand*>(rundownQueueWidget)->executeCommand(Playout::PlayoutType::Next);

                this->currentAutoPlayWidget = rundownQueueWidget;
            }

            break;
        }
    }
}

void RundownTreeWidget::setActive(bool active)
{
    this->active = active;

    if (this->active)
    {
        EventManager::getInstance().fireAllowRemoteTriggeringEvent(AllowRemoteTriggeringEvent(this->allowRemoteRundownTriggering));
        EventManager::getInstance().fireRepositoryRundownEvent(RepositoryRundownEvent(this->repositoryRundown));
    }

    EventManager::getInstance().fireActiveRundownChangedEvent(ActiveRundownChangedEvent(this->activeRundown));

    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);

    QTreeWidgetItem* currentItemParent = NULL;
    if (currentItem != NULL)
        currentItemParent = this->treeWidgetRundown->currentItem()->parent();

    QWidget* currentItemWidgetParent = NULL;
    if (currentItemParent != NULL)
        currentItemWidgetParent = this->treeWidgetRundown->itemWidget(currentItemParent, 0);

    if (currentItem != NULL && currentItemWidget != NULL)
    {
        dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->setActive(this->active);
        dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->setSelected(this->active);

        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(true));
    }
    else // Empty rundown.
    {
        EventManager::getInstance().fireEmptyRundownEvent(EmptyRundownEvent());
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(false));
    }
}

void RundownTreeWidget::openRundown(const QString& path)
{
    QElapsedTimer time;
    time.start();

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Opening rundown..."));

    qDebug("Open rundown %s", qPrintable(path));

    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        this->activeRundown = path;

        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);

        // Save the latest value stored in the clipboard.
        QString latest = qApp->clipboard()->text();
        QString data = stream.readAll();

        this->hexHash = QString(QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5).toHex());
        qDebug("Hash is %s", qPrintable(this->hexHash));

        qApp->clipboard()->setText(data);
        pasteSelectedItems();

        // Set previous stored clipboard value.
        qApp->clipboard()->setText(latest);

        qDebug("Parsing rundown completed in %lld msec", time.elapsed());

        file.close();

        if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
            this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

        this->treeWidgetRundown->setFocus();

        DatabaseManager::getInstance().insertOpenRecent(path);

        qDebug("RundownTreeWidget::openRundown %lld msec (%d items)", time.elapsed(), this->treeWidgetRundown->invisibleRootItem()->childCount());
    }

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));
}

void RundownTreeWidget::openRundownFromUrl(const QString& url)
{
    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Opening rundown..."));
    EventManager::getInstance().fireReloadRundownMenuEvent(ReloadRundownMenuEvent(false));

    qDebug("Open rundown from url %s", qPrintable(url));

    this->activeRundown = url;

    this->repositoryDevice = QSharedPointer<RepositoryDevice>(new RepositoryDevice(QUrl(url).host()));
    QObject::connect(this->repositoryDevice.data(), SIGNAL(connectionStateChanged(RepositoryDevice&)), this, SLOT(repositoryConnectionStateChanged(RepositoryDevice&)));
    QObject::connect(this->repositoryDevice.data(), SIGNAL(repositoryChanged(const RepositoryChangeModel&, RepositoryDevice&)), this, SLOT(repositoryChanged(const RepositoryChangeModel&, RepositoryDevice&)));
    this->repositoryDevice->connectDevice();

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doOpenRundownFromUrl(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(QUrl(url)));
}

void RundownTreeWidget::doOpenRundownFromUrl(QNetworkReply* reply)
{
    this->repositoryRundown = true;

    // Save the latest value stored in the clipboard.
    QString latest = qApp->clipboard()->text();
    QString data = QString::fromUtf8(reply->readAll());

    this->hexHash = QString(QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5).toHex());
    qDebug("Hash is %s", qPrintable(this->hexHash));

    qApp->clipboard()->setText(data);
    pasteSelectedItems();

    // Set previous stored clipboard value.
    qApp->clipboard()->setText(latest);

    if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
        this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

    this->treeWidgetRundown->setFocus();

    EventManager::getInstance().fireSaveMenuEvent(SaveMenuEvent(false));
    EventManager::getInstance().fireSaveAsMenuEvent(SaveAsMenuEvent(false));
    EventManager::getInstance().fireReloadRundownMenuEvent(ReloadRundownMenuEvent(true));
    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));

    reply->deleteLater();
    this->networkManager->deleteLater();
}

void RundownTreeWidget::repositoryConnectionStateChanged(RepositoryDevice& device)
{
    qDebug("RundownTreeWidget::repositoryConnectionStateChanged %s (%s)", qPrintable(device.getAddress()), qPrintable((device.isConnected() == true) ? "connected" : "disconnected"));

    QStringList repositoryUrl = this->activeRundown.split("/");
    QString rundown = repositoryUrl.takeLast();
    QString profile = repositoryUrl.takeLast();

    device.subscribe(rundown, profile);
}

void RundownTreeWidget::repositoryChanged(const RepositoryChangeModel& model, RepositoryDevice& device)
{
    Q_UNUSED(device);

    this->treeWidgetRundown->addRepositoryChange(model);
    this->treeWidgetRundown->applyRepositoryChanges();
}

void RundownTreeWidget::insertRepositoryChanges(const InsertRepositoryChangesEvent& event)
{
    Q_UNUSED(event);

    if (!this->active)
        return;

    this->treeWidgetRundown->applyRepositoryChanges();
    this->treeWidgetRundown->checRepositoryChanges();
}

void RundownTreeWidget::reloadRundown()
{
    if (this->activeRundown == Rundown::DEFAULT_NAME)
        return;

    if (this->copyItem != NULL)
        this->copyItem = NULL;

    if (this->currentPlayingItem != NULL)
        this->currentPlayingItem = NULL;

    if (this->currentPlayingAutoStepItem != NULL)
        this->currentPlayingAutoStepItem = NULL;

    if (this->currentAutoPlayWidget != NULL)
        this->currentAutoPlayWidget = NULL;

    this->treeWidgetRundown->removeAllItems();

    if (this->repositoryRundown)
        openRundownFromUrl(this->activeRundown);
    else
        openRundown(this->activeRundown);

    if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
        this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));
}

void RundownTreeWidget::saveRundown(bool saveAs)
{
    if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
        return;

    QString path;
    if (saveAs) {
        if (this->activeRundown == Rundown::DEFAULT_NAME) {
            path = QDir::homePath();
        }
        else {
            QFileInfo fi(this->activeRundown);
            path = fi.absolutePath();
        }
        path = QFileDialog::getSaveFileName(this, "Save Rundown", path, "Rundown (*.xml)");
    }
    else
        path = (this->activeRundown == Rundown::DEFAULT_NAME) ? QFileDialog::getSaveFileName(this, "Save Rundown", QDir::homePath(), "Rundown (*.xml)") : this->activeRundown;

    if (!path.isEmpty())
    {
        // Make sure we have an extension. On *nix system it will not be appended by default.
        if (!path.toLower().endsWith(".xml"))
            path.append(".xml");
        EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Saving rundown..."));

        QFile file(path);
        if (file.exists())
            file.remove();

        if (file.open(QFile::WriteOnly))
        {
            QByteArray data;
            QXmlStreamWriter writer(&data);

            writer.setAutoFormatting(XmlFormatting::ENABLE_FORMATTING);
            writer.setAutoFormattingIndent(XmlFormatting::NUMBER_OF_SPACES);

            writer.writeStartDocument();
            writer.writeStartElement("items");
            writer.writeTextElement("allowremotetriggering", (this->allowRemoteRundownTriggering == true) ? "true" : "false");

            for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
                this->treeWidgetRundown->writeProperties(this->treeWidgetRundown->invisibleRootItem()->child(i), writer);

            writer.writeEndElement();
            writer.writeEndDocument();

            this->hexHash = QString(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
            qDebug("Hash is %s", qPrintable(this->hexHash));

            file.write(data);
            file.close();

            DatabaseManager::getInstance().insertOpenRecent(path);

            qDebug("Saved rundown to %s", qPrintable(path));
        }

        this->activeRundown = path;
        EventManager::getInstance().fireActiveRundownChangedEvent(ActiveRundownChangedEvent(this->activeRundown));
        EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));
    }
}

bool RundownTreeWidget::checkForSave() const
{
    // Don't save empty rundowns.
    if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
        return false;

    // We can't save repository rundowns.
    if (this->repositoryRundown)
        return false;

    QByteArray data;
    QXmlStreamWriter writer(&data);

    writer.setAutoFormatting(XmlFormatting::ENABLE_FORMATTING);
    writer.setAutoFormattingIndent(XmlFormatting::NUMBER_OF_SPACES);

    writer.writeStartDocument();
    writer.writeStartElement("items");
    writer.writeTextElement("allowremotetriggering", (this->allowRemoteRundownTriggering == true) ? "true" : "false");

    for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
        this->treeWidgetRundown->writeProperties(this->treeWidgetRundown->invisibleRootItem()->child(i), writer);

    writer.writeEndElement();
    writer.writeEndDocument();

    QString hexHash = QString(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
    qDebug("Hash is %s", qPrintable(hexHash));

    if (hexHash != this->hexHash)
        return true;

    return false;
}

void RundownTreeWidget::colorizeItems(const QString& color)
{
    if (this->treeWidgetRundown->selectedItems().count() == 0)
        return;

    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
        dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->setColor(color); // Colorize current selected item.
}

void RundownTreeWidget::gpiPortTriggered(int gpiPort, GpiDevice* device)
{
    Q_UNUSED(device);

    executeCommand(gpiBindings[gpiPort], Action::ActionType::GpiPulse);
}

void RundownTreeWidget::gpiBindingChanged(int gpiPort, Playout::PlayoutType binding)
{
    gpiBindings[gpiPort] = binding;
}

void RundownTreeWidget::customContextMenuRequested(const QPoint& point)
{
    foreach (QAction* action, this->contextMenuRundown->actions())
        action->setEnabled(true);

    bool isGroup = false;
    bool isTopItem = false;
    bool isGroupItem = false;
    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
    {
        QWidget* widget = this->treeWidgetRundown->itemWidget(item, 0);

        if (item->parent() != NULL) // Group item.
            isGroupItem = true;
        else if (dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Group
            isGroup = true;
        else if (item->parent() == NULL && !dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Top level item.
            isTopItem = true;
    }

    if (isGroup)
        this->contextMenuRundown->actions().at(2)->setEnabled(false); // We don't support group in groups.

    if (isGroupItem)
        this->contextMenuRundown->actions().at(2)->setEnabled(false); // We don't support group in groups.

    if (isTopItem || (isGroup && isGroupItem) || (isTopItem && isGroupItem))
        this->contextMenuRundown->actions().at(3)->setEnabled(false); // We don't have any group to ungroup.

    if (!isTopItem && !isGroup && !isGroupItem)
    {
        this->contextMenuRundown->actions().at(2)->setEnabled(false); // Group.
        this->contextMenuRundown->actions().at(3)->setEnabled(false); // Ungroup.
        this->contextMenuRundown->actions().at(5)->setEnabled(false); // Mark Item.
        this->contextMenuRundown->actions().at(7)->setEnabled(false); // Copy Properties.
        this->contextMenuRundown->actions().at(8)->setEnabled(false); // Paste Properties.
        this->contextMenuRundown->actions().at(10)->setEnabled(false); // Colorize Item.
        this->contextMenuRundown->actions().at(12)->setEnabled(false); // Save as Preset.
        this->contextMenuRundown->actions().at(14)->setEnabled(false); // Remove.
    }

    if (this->repositoryRundown)
    {
        this->contextMenuRundown->actions().at(0)->setEnabled(false); // Tools.
        this->contextMenuRundown->actions().at(2)->setEnabled(false); // Group.
        this->contextMenuRundown->actions().at(3)->setEnabled(false); // Ungroup.
        this->contextMenuRundown->actions().at(8)->setEnabled(false); // Paste Properties.
        this->contextMenuRundown->actions().at(10)->setEnabled(false); // Colorize Item.
        this->contextMenuRundown->actions().at(12)->setEnabled(false); // Save as Preset.
        this->contextMenuRundown->actions().at(14)->setEnabled(false); // Remove.
    }

    if (this->treeWidgetRundown->selectedItems().count() > 0)
    {
        if (this->treeWidgetRundown->selectedItems().count() == 1)
        {
            QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
            QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);
            if (dynamic_cast<AbstractRundownWidget*>(currentItemWidget) != NULL)
            {
                QString color = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getColor();
                QString name = colorLookup(color, true);

                foreach (QAction* action, this->contextMenuColor->actions())
                {
                    if (action->text() == name)
                        action->setChecked(true);
                    else
                        action->setChecked(false);
                }
            }
        }
        else
        {
            foreach (QAction* action, this->contextMenuColor->actions())
                action->setChecked(false);
        }
    }

    this->contextMenuRundown->exec(this->treeWidgetRundown->mapToGlobal(point));
}

void RundownTreeWidget::contextMenuNewTriggered(QAction* action)
{
    Q_UNUSED(action);
}

QString RundownTreeWidget::colorLookup(const QString& color, bool reverse) const
{
    if (reverse)
    {
        if (color == Color::SIENNA_COLOR)
            return "Sienna";
        else if (color == Color::OLIVEDRAB_COLOR)
            return "OliveDrab";
        else if (color == Color::SEAGREEN_COLOR)
            return "SeaGreen";
        else if (color == Color::CHOCOLATE_COLOR)
            return "Chocolate";
        else if (color == Color::DARKSLATEGRAY_COLOR)
            return "DarkSlateGray";
        else if (color == Color::STEELBLUE_COLOR)
            return "SteelBlue";
        else if (color == Color::MAROON_COLOR)
            return "Maroon";
        else if (color == Color::MAROONLIGHT_COLOR)
            return "MaroonLight";
        else if (color == Color::DARKKHAKI_COLOR)
            return "DarkKhaki";
        else if (color == Color::ROYALBLUE_COLOR)
            return "RoyalBlue";
    }
    else
    {
        if (color == "Sienna")
            return Color::SIENNA_COLOR;
        else if (color == "OliveDrab")
            return Color::OLIVEDRAB_COLOR;
        else if (color == "SeaGreen")
            return Color::SEAGREEN_COLOR;
        else if (color == "Chocolate")
            return Color::CHOCOLATE_COLOR;
        else if (color == "DarkSlateGray")
            return Color::DARKSLATEGRAY_COLOR;
        else if (color == "SteelBlue")
            return Color::STEELBLUE_COLOR;
        else if (color == "Maroon")
            return Color::MAROON_COLOR;
        else if (color == "MaroonLight")
            return Color::MAROONLIGHT_COLOR;
        else if (color == "DarkKhaki")
            return Color::DARKKHAKI_COLOR;
        else if (color == "RoyalBlue")
            return Color::ROYALBLUE_COLOR;
    }

    return ""; // Reset
}

void RundownTreeWidget::contextMenuColorTriggered(QAction* action)
{
    colorizeItems(colorLookup(action->text(), false));
}

void RundownTreeWidget::contextMenuRundownTriggered(QAction* action)
{
    if (action->text() == "Group")
        this->treeWidgetRundown->groupItems();
    else if (action->text() == "Ungroup")
        this->treeWidgetRundown->ungroupItems();
}

void RundownTreeWidget::itemSelectionChanged()
{
    if (this->treeWidgetRundown->currentItem() == NULL || this->treeWidgetRundown->selectedItems().count() == 0)
    {
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(false));
        return;
    }

    EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(true));
}

void RundownTreeWidget::itemClicked(QTreeWidgetItem* current, int index)
{
    Q_UNUSED(current);
    Q_UNUSED(index);

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);

    QTreeWidgetItem* currentItemParent = NULL;
    if (currentItem != NULL)
        currentItemParent = this->treeWidgetRundown->currentItem()->parent();

    QWidget* currentItemWidgetParent = NULL;
    if (currentItemParent != NULL)
        currentItemWidgetParent = this->treeWidgetRundown->itemWidget(currentItemParent, 0);

    if (currentItem != NULL && currentItemWidget != NULL)
    {
        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(true));
    }
}

void RundownTreeWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    QWidget* currentWidget = this->treeWidgetRundown->itemWidget(current, 0);
    QWidget* previousWidget = this->treeWidgetRundown->itemWidget(previous, 0);

    if (previous != NULL && previousWidget != NULL)
    {
        dynamic_cast<AbstractRundownWidget*>(previousWidget)->setSelected(false);

        if (previous != this->currentPlayingItem && previous != this->currentPlayingAutoStepItem)
            dynamic_cast<AbstractRundownWidget*>(previousWidget)->setActive(false);
    }

    if (current != NULL && currentWidget != NULL)
    {
        dynamic_cast<AbstractRundownWidget*>(currentWidget)->setSelected(true);

        if (current != this->currentPlayingItem && current != this->currentPlayingAutoStepItem)
            dynamic_cast<AbstractRundownWidget*>(currentWidget)->setActive(true);
    }

    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);

    QTreeWidgetItem* currentItemParent = NULL;
    if (currentItem != NULL)
        currentItemParent = this->treeWidgetRundown->currentItem()->parent();

    QWidget* currentItemWidgetParent = NULL;
    if (currentItemParent != NULL)
        currentItemWidgetParent = this->treeWidgetRundown->itemWidget(currentItemParent, 0);

    if (currentItem != NULL && currentItemWidget != NULL)
    {
        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(true));
    }
    else if (currentItem == NULL && previous != NULL && this->treeWidgetRundown->invisibleRootItem()->childCount() == 1) // Last item was removed form the rundown.
    {
        EventManager::getInstance().fireEmptyRundownEvent(EmptyRundownEvent());
        EventManager::getInstance().fireSaveAsPresetMenuEvent(SaveAsPresetMenuEvent(false));
    }
}

void RundownTreeWidget::itemDoubleClicked(QTreeWidgetItem* item, int index)
{
    Q_UNUSED(index);

    QWidget* selectedWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0);
    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);

    if (rundownWidget->isGroup()) // Group.
        rundownWidget->setExpanded(!item->isExpanded());
}

bool RundownTreeWidget::duplicateSelectedItems()
{
    return this->treeWidgetRundown->duplicateSelectedItems();
}

bool RundownTreeWidget::pasteSelectedItems()
{
    return this->treeWidgetRundown->pasteSelectedItems(this->repositoryRundown);
}

bool RundownTreeWidget::copySelectedItems() const
{
    return this->treeWidgetRundown->copySelectedItems();
}

void RundownTreeWidget::setUsed(bool used)
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    QWidget* selectedWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0);
    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);

    rundownWidget->setUsed(used);
    if (rundownWidget != NULL && rundownWidget->isGroup())
    {
        for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
        {
            QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem()->child(i), 0);

            dynamic_cast<AbstractRundownWidget*>(childWidget)->setUsed(used);
        }
    }
}

void RundownTreeWidget::setAllUsed(bool used)
{
    for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
    {
        QTreeWidgetItem* currentItem = this->treeWidgetRundown->invisibleRootItem()->child(i);
        AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0));

        rundownWidget->setUsed(used);
        if (rundownWidget != NULL && rundownWidget->isGroup())
        {
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->child(i), 0);

                dynamic_cast<AbstractRundownWidget*>(childWidget)->setUsed(used);
            }
        }
    }
}

bool RundownTreeWidget::executeCommand(Playout::PlayoutType type, Action::ActionType source, QTreeWidgetItem* item)
{
    //QModelIndex currentIndex;
    QTreeWidgetItem* currentItem = nullptr;

    QWidget* selectedWidget = nullptr;
    QWidget* selectedWidgetParent = nullptr;

    AbstractRundownWidget* rundownWidget = nullptr;
    AbstractRundownWidget* rundownWidgetParent = nullptr;

    if (item == nullptr)
    {
        if (this->treeWidgetRundown->currentItem() == nullptr)
            return true;

        currentItem = this->treeWidgetRundown->currentItem();
        //currentIndex = this->treeWidgetRundown->currentIndex();

        selectedWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);
        selectedWidgetParent = this->treeWidgetRundown->itemWidget(currentItem->parent(), 0);

        rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);
        rundownWidgetParent = dynamic_cast<AbstractRundownWidget*>(selectedWidgetParent);
    }
    else // External execution through OSC.
    {
        currentItem = item;
        //currentIndex = this->treeWidgetRundown->indexOfTopLevelItem(item);

        selectedWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);
        selectedWidgetParent = this->treeWidgetRundown->itemWidget(currentItem->parent(), 0);

        rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);
        rundownWidgetParent = dynamic_cast<AbstractRundownWidget*>(selectedWidgetParent);
    }

    if (source == Action::ActionType::GpiPulse && !rundownWidget->getCommand()->getAllowGpi())
        return true; // Gpi pulses cannot trigger this item.

    if (type == Playout::PlayoutType::Next && rundownWidgetParent != nullptr && rundownWidgetParent->isGroup() && dynamic_cast<GroupCommand*>(rundownWidgetParent->getCommand())->getAutoPlay())
    {
        EventManager::getInstance().fireAutoPlayNextRundownItemEvent(AutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget)));

        return true;
    }
    else
    {
        if (this->currentPlayingItem != nullptr)
            dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->currentPlayingItem, 0))->setActive(false);

        dynamic_cast<AbstractRundownWidget*>(selectedWidget)->setActive(true);
        dynamic_cast<AbstractPlayoutCommand*>(selectedWidget)->executeCommand(type);

        this->currentPlayingItem = currentItem;
    }

    if (rundownWidget != nullptr && rundownWidget->isGroup())
    {
        if (type == Playout::PlayoutType::Next && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
        {
            if (this->currentAutoPlayWidget != nullptr)
                EventManager::getInstance().fireAutoPlayNextRundownItemEvent(AutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget)));
        }
        else if (type == Playout::PlayoutType::PauseResume && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
        {
            if (this->currentAutoPlayWidget != nullptr)
                dynamic_cast<AbstractPlayoutCommand*>(this->currentAutoPlayWidget)->executeCommand(type);
        }
        else if ((type == Playout::PlayoutType::Play || type == Playout::PlayoutType::Load) && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
        {
            // The group have AutoPlay enabled, play the items within the group.
            bool isFirstChild = true;

            QList<AbstractRundownWidget*>* autoPlayQueue = new QList<AbstractRundownWidget*>();
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->child(i), 0);
                AbstractRundownWidget* rundownChildWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);
                if (dynamic_cast<MovieCommand*>(rundownChildWidget->getCommand()))
                {
                    // Skip video items without AutoPlay.
                    if (!dynamic_cast<MovieCommand*>(rundownChildWidget->getCommand())->getAutoPlay())
                        continue;

                    // Only execute the first child in the group, add the rest to the AutoPlay queue.
                    if (isFirstChild)
                    {
                        dynamic_cast<AbstractPlayoutCommand*>(rundownChildWidget)->executeCommand(type);
                        if (type == Playout::PlayoutType::Load)
                            break; // We only want  to load the first item.

                        this->currentAutoPlayWidget = rundownChildWidget;
                    }

                    autoPlayQueue->push_back(rundownChildWidget); // Add our widget to the execution queue.

                    isFirstChild = false;
                }
                else
                {
                    dynamic_cast<AbstractPlayoutCommand*>(rundownChildWidget)->executeCommand(type);
                }
            }

            if (autoPlayQueue->count() > 0)
                this->autoPlayQueues.push_back(autoPlayQueue);
        }
        else
        {
            // Setting: Should we clear delayed commands on play if the group have AutoStep property set.
            if (this->clearDelayedCommandsOnAutoStep)
            {
                if (dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoStep() && type == Playout::PlayoutType::Play)
                {
                    EventManager::getInstance().fireClearDelayedCommands();

                    this->currentPlayingAutoStepItem = currentItem;
                }
            }

            // Execute command on the selected item.
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->child(i), 0);

                EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(currentItem->child(i)));

                dynamic_cast<AbstractPlayoutCommand*>(childWidget)->executeCommand(type);         
            }

            if (type == Playout::PlayoutType::Preview)
                return true; // We are done.
        }

        // Setting: Should we AutoStep and send Preview on next item.
        if ((type != Playout::PlayoutType::Clear &&
             type != Playout::PlayoutType::ClearVideoLayer &&
             type != Playout::PlayoutType::ClearChannel) &&
             dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoStep())
        { 
            QTreeWidgetItem* previousItem = this->treeWidgetRundown->currentItem();

            selectItemBelow();
            //QTimer::singleShot(500, this, SLOT(selectItemBelow()));

            if (this->treeWidgetRundown->currentItem() != previousItem)
            {
                if (this->previewOnAutoStep && type == Playout::PlayoutType::Play)
                {
                    executePreview();
                    //QTimer::singleShot(600, this, SLOT(executePreview()));
                }
            }
        }
    }
    else if (rundownWidgetParent != nullptr && rundownWidgetParent->isGroup())
    {
        // The selected items parent is a group. If the group have AutoPlay property set, then play current item and below within the group.
        if (type == Playout::PlayoutType::Play && dynamic_cast<GroupCommand*>(rundownWidgetParent->getCommand())->getAutoPlay())
        {
            QList<AbstractRundownWidget*>* autoPlayQueue = new QList<AbstractRundownWidget*>();
            for (int i = currentItem->parent()->indexOfChild(currentItem); i < currentItem->parent()->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->parent()->child(i), 0);
                AbstractRundownWidget* rundownChildWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);
                if (dynamic_cast<MovieCommand*>(rundownChildWidget->getCommand()))
                {
                    // Skip video items without AutoPlay.
                    if (!dynamic_cast<MovieCommand*>(rundownChildWidget->getCommand())->getAutoPlay())
                        continue;

                    autoPlayQueue->push_back(rundownChildWidget); // Add our widget to the execution queue.
                }
            }

            this->autoPlayQueues.push_back(autoPlayQueue);
        }
    }

    return true;
}

void RundownTreeWidget::selectItemBelow()
{
    this->treeWidgetRundown->selectItemBelow();
}

void RundownTreeWidget::executePreview()
{
    EventManager::getInstance().fireExecutePlayoutCommandEvent(ExecutePlayoutCommandEvent(QEvent::KeyPress, Qt::Key_F8, Qt::NoModifier));
}

void RundownTreeWidget::addBlendModeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::BLENDMODE);
}

void RundownTreeWidget::addBrightnessItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::BRIGHTNESS);
}

void RundownTreeWidget::addContrastItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CONTRAST);
}

void RundownTreeWidget::addClipItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CLIP);
}

void RundownTreeWidget::addCropItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CROP);
}

void RundownTreeWidget::addImageScrollerItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::IMAGESCROLLER);
}

void RundownTreeWidget::addDeckLinkInputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::DECKLINKINPUT);
}

void RundownTreeWidget::addPrintItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::PRINT);
}

void RundownTreeWidget::addClearOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CLEAROUTPUT);
}

void RundownTreeWidget::addFillItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::FILL);
}

void RundownTreeWidget::addGpiOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GPIOUTPUT);
}

void RundownTreeWidget::addHttpGetItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::HTTPGET);
}

void RundownTreeWidget::addHttpPostItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::HTTPPOST);
}

void RundownTreeWidget::addOscOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::OSCOUTPUT);
}

void RundownTreeWidget::addPlayoutCommandItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::PLAYOUTCOMMAND);
}

void RundownTreeWidget::addRouteChannelItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ROUTECHANNEL);
}

void RundownTreeWidget::addRouteVideolayerItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ROUTEVIDEOLAYER);
}

void RundownTreeWidget::addFileRecorderItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::FILERECORDER);
}

void RundownTreeWidget::addSeparatorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SEPARATOR);
}


void RundownTreeWidget::addGridItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GRID);
}

void RundownTreeWidget::addCustomCommandItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CUSTOMCOMMAND);
}

void RundownTreeWidget::addChromaKeyItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::CHROMAKEY);
}

void RundownTreeWidget::addSolidColorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SOLIDCOLOR);
}

void RundownTreeWidget::addHtmlItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::HTML);
}

void RundownTreeWidget::addPerspectiveItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::PERSPECTIVE);
}

void RundownTreeWidget::addRotationItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ROTATION);
}

void RundownTreeWidget::addAnchorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ANCHOR);
}

void RundownTreeWidget::addFadeToBlackItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::FADETOBLACK);
}

void RundownTreeWidget::addKeyerItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::KEYER);
}

void RundownTreeWidget::addLevelsItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::LEVELS);
}

void RundownTreeWidget::addOpacityItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::OPACITY);
}

void RundownTreeWidget::addResetItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::RESET);
}

void RundownTreeWidget::addSaturationItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SATURATION);
}

void RundownTreeWidget::addVolumeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::VOLUME);
}

void RundownTreeWidget::addCommitItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::COMMIT);
}

void RundownTreeWidget::addAudioItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::AUDIO);
}

void RundownTreeWidget::addImageItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::STILL);
}

void RundownTreeWidget::addTemplateItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TEMPLATE);
}

void RundownTreeWidget::addVideoItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::MOVIE);
}


void RundownTreeWidget::saveAsPreset()
{
    if (!copySelectedItems())
        return;

    PresetDialog* dialog = new PresetDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertPreset(PresetModel(0, dialog->getName(), qApp->clipboard()->text()));
        EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
    }
}

void RundownTreeWidget::removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent& event)
{
    AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(event.getItem(), 0));
    foreach (QList<AbstractRundownWidget*>* autoPlayQueue, this->autoPlayQueues)
    {
        if (autoPlayQueue->contains(widget))
        {
            autoPlayQueue->removeOne(widget);

            if (autoPlayQueue->isEmpty())
                this->autoPlayQueues.removeOne(autoPlayQueue);

            break;
        }
    }
}

void RundownTreeWidget::clearCurrentPlayingItem(const ClearCurrentPlayingItemEvent& event)
{
    if (this->currentPlayingItem == event.getItem())
        this->currentPlayingItem = NULL;

    if (this->currentPlayingAutoStepItem == event.getItem())
        this->currentPlayingAutoStepItem = NULL;
}

void RundownTreeWidget::currentItemChanged(const CurrentItemChangedEvent& event)
{
    currentItemChanged(event.getCurrentItem(), event.getPreviousItem());
}

bool RundownTreeWidget::getAllowRemoteTriggering() const
{
    return this->allowRemoteRundownTriggering;
}

void RundownTreeWidget::resetOscSubscriptions()
{
    if (this->upControlSubscription != NULL)
        this->upControlSubscription->disconnect(); // Disconnect all events.

    if (this->downControlSubscription != NULL)
        this->downControlSubscription->disconnect(); // Disconnect all events.

    if (this->playAndAutoStepControlSubscription != NULL)
        this->playAndAutoStepControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowAndAutoStepControlSubscription != NULL)
        this->playNowAndAutoStepControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowIfChannelControlSubscription != NULL)
        this->playNowIfChannelControlSubscription->disconnect(); // Disconnect all events.

    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->playNowControlSubscription != NULL)
        this->playNowControlSubscription->disconnect(); // Disconnect all events.

    if (this->loadControlSubscription != NULL)
        this->loadControlSubscription->disconnect(); // Disconnect all events.

    if (this->pauseControlSubscription != NULL)
        this->pauseControlSubscription->disconnect(); // Disconnect all events.

    if (this->nextControlSubscription != NULL)
        this->nextControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

    if (this->previewControlSubscription != NULL)
        this->previewControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.
}

void RundownTreeWidget::configureOscSubscriptions()
{
    resetOscSubscriptions();

    QFileInfo path(this->activeRundown);

    QString upControlFilter = Osc::RUNDOWN_CONTROL_UP_FILTER;
    this->upControlSubscription = new OscSubscription(upControlFilter, this);
    QObject::connect(this->upControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(upControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString downControlFilter = Osc::RUNDOWN_CONTROL_DOWN_FILTER;
    this->downControlSubscription = new OscSubscription(downControlFilter, this);
    QObject::connect(this->downControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(downControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playAndAutoStepControlFilter = Osc::RUNDOWN_CONTROL_PLAYANDAUTOSTEP_FILTER;
    this->playAndAutoStepControlSubscription = new OscSubscription(playAndAutoStepControlFilter, this);
    QObject::connect(this->playAndAutoStepControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playAndAutoStepControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playNowAndAutoStepControlFilter = Osc::RUNDOWN_CONTROL_PLAYNOWANDAUTOSTEP_FILTER;
    this->playNowAndAutoStepControlSubscription = new OscSubscription(playNowAndAutoStepControlFilter, this);
    QObject::connect(this->playNowAndAutoStepControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowAndAutoStepControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playNowIfChannelControlFilter = Osc::RUNDOWN_CONTROL_PLAYNOWIFCHANNEL_FILTER;
    this->playNowIfChannelControlSubscription = new OscSubscription(playNowIfChannelControlFilter, this);
    QObject::connect(this->playNowIfChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowIfChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString stopControlFilter = Osc::RUNDOWN_CONTROL_STOP_FILTER;
    this->stopControlSubscription = new OscSubscription(stopControlFilter, this);
    QObject::connect(this->stopControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(stopControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playControlFilter = Osc::RUNDOWN_CONTROL_PLAY_FILTER;
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playNowControlFilter = Osc::RUNDOWN_CONTROL_PLAYNOW_FILTER;
    this->playNowControlSubscription = new OscSubscription(playNowControlFilter, this);
    QObject::connect(this->playNowControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::RUNDOWN_CONTROL_LOAD_FILTER;
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString pauseControlFilter = Osc::RUNDOWN_CONTROL_PAUSE_FILTER;
    this->pauseControlSubscription = new OscSubscription(pauseControlFilter, this);
    QObject::connect(this->pauseControlSubscription, SIGNAL(subscriptionReceived(const QString &, const QList<QVariant> &)),
                     this, SLOT(pauseControlSubscriptionReceived(const QString &, const QList<QVariant> &)));
                     
    QString nextControlFilter = Osc::RUNDOWN_CONTROL_NEXT_FILTER;
    this->nextControlSubscription = new OscSubscription(nextControlFilter, this);
    QObject::connect(this->nextControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(nextControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString updateControlFilter = Osc::RUNDOWN_CONTROL_UPDATE_FILTER;
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString invokeControlFilter = Osc::RUNDOWN_CONTROL_INVOKE_FILTER;
    this->invokeControlSubscription = new OscSubscription(invokeControlFilter, this);
    QObject::connect(this->invokeControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString previewControlFilter = Osc::RUNDOWN_CONTROL_PREVIEW_FILTER;
    this->previewControlSubscription = new OscSubscription(previewControlFilter, this);
    QObject::connect(this->previewControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(previewControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearControlFilter = Osc::RUNDOWN_CONTROL_CLEAR_FILTER;
    this->clearControlSubscription = new OscSubscription(clearControlFilter, this);
    QObject::connect(this->clearControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearVideolayerControlFilter = Osc::RUNDOWN_CONTROL_CLEARVIDEOLAYER_FILTER;
    this->clearVideolayerControlSubscription = new OscSubscription(clearVideolayerControlFilter, this);
    QObject::connect(this->clearVideolayerControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearChannelControlFilter = Osc::RUNDOWN_CONTROL_CLEARCHANNELFILTER;
    this->clearChannelControlSubscription = new OscSubscription(clearChannelControlFilter, this);
    QObject::connect(this->clearChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownTreeWidget::upControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
    {
        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemAbove();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::downControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
    {
        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemBelow();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::playAndAutoStepControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
    {
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Play, this->treeWidgetRundown->currentItem()));

        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemBelow();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::playNowAndAutoStepControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
    {
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::PlayNow, this->treeWidgetRundown->currentItem()));

        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemBelow();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::playNowIfChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0)
    {
        QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
        AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0));

        if (rundownWidget != NULL && rundownWidget->isGroup())
        {
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QTreeWidgetItem* childItem = currentItem->child(i);
                AbstractRundownWidget* rundownChildWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(childItem, 0));

                LibraryModel* model = rundownChildWidget->getLibraryModel();
                if (model == NULL)
                    continue;

                const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(model->getDeviceName());
                if (device == NULL)
                    continue; // Only CasparCG devices.

                AbstractCommand* command = dynamic_cast<AbstractCommand*>(rundownChildWidget->getCommand());
                if (arguments[0].toInt() == command->getChannel())
                    EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::PlayNow, childItem));
            }
        }
        else
        {
            LibraryModel* model = rundownWidget->getLibraryModel();
            if (model == NULL)
                return;

            const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(model->getDeviceName());
            if (device == NULL)
                return; // Only CasparCG devices.

            AbstractCommand* command = dynamic_cast<AbstractCommand*>(rundownWidget->getCommand());
            if (arguments[0].toInt() == command->getChannel())
                EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::PlayNow, currentItem));
        }
    }
}

void RundownTreeWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Stop, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Play, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::playNowControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::PlayNow, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Load, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::PauseResume, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Next, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Update, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::invokeControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Invoke, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::previewControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0 && !this->previewOnAutoStep) 
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Preview, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Clear, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::ClearVideoLayer, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    if (!this->active)
        return;

    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    if (this->allowRemoteRundownTriggering && arguments.count() > 0 && arguments[0].toInt() > 0)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::ClearChannel, this->treeWidgetRundown->currentItem()));
}
