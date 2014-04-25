#include "RundownTreeWidget.h"
#include "RundownBlendModeWidget.h"
#include "RundownBrightnessWidget.h"
#include "RundownCommitWidget.h"
#include "RundownContrastWidget.h"
#include "RundownCropWidget.h"
#include "RundownDeckLinkInputWidget.h"
#include "RundownFileRecorderWidget.h"
#include "RundownImageScrollerWidget.h"
#include "RundownGeometryWidget.h"
#include "RundownGpiOutputWidget.h"
#include "RundownGridWidget.h"
#include "RundownGroupWidget.h"
#include "RundownKeyerWidget.h"
#include "RundownLevelsWidget.h"
#include "RundownVideoWidget.h"
#include "RundownOpacityWidget.h"
#include "RundownSaturationWidget.h"
#include "RundownTemplateWidget.h"
#include "RundownVolumeWidget.h"
#include "RundownSeparatorWidget.h"
#include "RundownPrintWidget.h"
#include "RundownClearOutputWidget.h"
#include "RundownSolidColorWidget.h"
#include "RundownAudioWidget.h"
#include "RundownImageWidget.h"
#include "RundownItemFactory.h"
#include "PresetDialog.h"

#include "GpiManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PresetChangedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/RundownModel.h"

#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QTextCodec>
#include <QtCore/QTime>
#include <QtCore/QTextStream>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtGui/QTreeWidgetItem>

RundownTreeWidget::RundownTreeWidget(QWidget* parent)
    : QWidget(parent),
      activeRundown(Rundown::DEFAULT_NAME), active(false), enterPressed(false), allowRemoteTriggering(false), currentAutoPlayWidget(NULL),
      copyItem(NULL), activeItem(NULL), currentPlayingAutoStepItem(NULL), upControlSubscription(NULL), downControlSubscription(NULL),
      stopControlSubscription(NULL), playControlSubscription(NULL), loadControlSubscription(NULL), pauseControlSubscription(NULL),
      nextControlSubscription(NULL), updateControlSubscription(NULL), invokeControlSubscription(NULL), clearControlSubscription(NULL),
      clearVideolayerControlSubscription(NULL), clearChannelControlSubscription(NULL)
{
    setupUi(this);
    setupMenus();

    QObject::connect(this->treeWidgetRundown, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    // TODO: Specific Gpi device.
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(gpiTriggered(int, GpiDevice*)), this, SLOT(gpiPortTriggered(int, GpiDevice*)));

    QObject::connect(&EventManager::getInstance(), SIGNAL(saveAsPreset(const SaveAsPresetEvent&)), this, SLOT(saveAsPreset(const SaveAsPresetEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(addPresetItem(const AddPresetItemEvent&)), this, SLOT(addPresetItem(const AddPresetItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(addRudnownItem(const AddRudnownItemEvent&)), this, SLOT(addRudnownItem(const AddRudnownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(toggleCompactView(const CompactViewEvent&)), this, SLOT(toggleCompactView(const CompactViewEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executeRundownItem(const ExecuteRundownItemEvent&)), this, SLOT(executeRundownItem(const ExecuteRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(remoteRundownTriggering(const RemoteRundownTriggeringEvent&)), this, SLOT(remoteRundownTriggering(const RemoteRundownTriggeringEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayRundownItem(const AutoPlayRundownItemEvent&)), this, SLOT(autoPlayRundownItem(const AutoPlayRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayChanged(const AutoPlayChangedEvent&)), this, SLOT(autoPlayChanged(const AutoPlayChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&)), this, SLOT(autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(executePlayoutCommand(const ExecutePlayoutCommandEvent&)), this, SLOT(executePlayoutCommand(const ExecutePlayoutCommandEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&)), this, SLOT(removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&)));

    foreach (const GpiPortModel& port, DatabaseManager::getInstance().getGpiPorts())
        gpiBindingChanged(port.getPort(), port.getAction());

    this->treeWidgetRundown->checkEmptyRundown();
}

RundownTreeWidget::~RundownTreeWidget()
{
}

void RundownTreeWidget::setupMenus()
{
    this->contextMenuMixer = new QMenu(this);
    this->contextMenuMixer->setObjectName("contextMenuMixer");
    this->contextMenuMixer->setTitle("Mixer");
    //this->contextMenuMixer->setIcon(QIcon(":/Graphics/Images/Mixer.png"));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BlendModeSmall.png"), "Blend Mode", this, SLOT(addBlendModeItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BrightnessSmall.png"), "Brightness", this, SLOT(addBrightnessItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ChromaSmall.png"), "Chroma Key", this, SLOT(addChromaKeyItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ContrastSmall.png"), "Contrast", this, SLOT(addContrastItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CropSmall.png"), "Crop", this, SLOT(addCropItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/GeometrySmall.png"), "Transformation", this, SLOT(addGeometryItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/GridSmall.png"), "Grid", this, SLOT(addGridItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/KeyerSmall.png"), "Mask", this, SLOT(addKeyerItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/LevelsSmall.png"), "Levels", this, SLOT(addLevelsItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/OpacitySmall.png"), "Opacity", this, SLOT(addOpacityItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/SaturationSmall.png"), "Saturation", this, SLOT(addSaturationItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/VolumeSmall.png"), "Volume", this, SLOT(addVolumeItem()));
    this->contextMenuMixer->addSeparator();
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CommitSmall.png"), "Commit", this, SLOT(addCommitItem()));

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
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/FileRecorderSmall.png"), "File Recorder", this, SLOT(addFileRecorderItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/GpiOutputSmall.png"), "GPI Output", this, SLOT(addGpiOutputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/OscOutputSmall.png"), "OSC Output", this, SLOT(addOscOutputItem()));
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SolidColorSmall.png"), "Solid Color", this, SLOT(addSolidColorItem()));
    this->contextMenuOther->addSeparator();
    this->contextMenuOther->addAction(QIcon(":/Graphics/Images/SeparatorSmall.png"), "Separator", this, SLOT(addSeparatorItem()));

    this->contextMenuTriCaster = new QMenu(this);
    this->contextMenuTriCaster->setObjectName("contextMenuTriCaster");
    this->contextMenuTriCaster->setTitle("TriCaster");
    //this->contextMenuTriCaster->setIcon(QIcon(":/Graphics/Images/TriCaster.png"));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/PlayMacroSmall.png"), "Play Macro", this, SLOT(addPlayMacroItem()));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/SelectInputSmall.png"), "Select Input", this, SLOT(addSelectInputItem()));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/SelectNetworkSourceSmall.png"), "Select Network Source", this, SLOT(addSelectNetworkSourceItem()));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/SelectPresetSmall.png"), "Select Preset", this, SLOT(addSelectPresetItem()));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/TriggerAutoSmall.png"), "Trigger Auto", this, SLOT(addTriggerAutoItem()));
    this->contextMenuTriCaster->addAction(QIcon(":/Graphics/Images/TriCaster/TriggerTakeSmall.png"), "Trigger Take", this, SLOT(addTriggerTakeItem()));

    this->contextMenuAtem = new QMenu(this);
    this->contextMenuAtem->setObjectName("contextMenuAtem");
    this->contextMenuAtem->setTitle("ATEM");
    //this->contextMenuAtem->setIcon(QIcon(":/Graphics/Images/Atem.png"));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/SelectInputSmall.png"), "Select Input", this, SLOT(addAtemSelectInputItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/AudioGainSmall.png"), "Set Audio Gain", this, SLOT(addAtemAudioInputGainItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/AudioInputStateSmall.png"), "Set Audio Input State", this, SLOT(addAtemAudioInputStateItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/AudioInputBalanceSmall.png"), "Set Audio Input Balance", this, SLOT(addAtemAudioInputBalanceItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/KeyerStateSmall.png"), "Set Keyer State", this, SLOT(addAtemKeyerStateItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/VideoFormatSmall.png"), "Set Video Format", this, SLOT(addAtemVideoFormatItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/TriggerAutoSmall.png"), "Trigger Auto", this, SLOT(addAtemTriggerAutoItem()));
    this->contextMenuAtem->addAction(QIcon(":/Graphics/Images/Atem/TriggerCutSmall.png"), "Trigger Cut", this, SLOT(addAtemTriggerCutItem()));

    this->contextMenuTools = new QMenu(this);
    this->contextMenuTools->setTitle("Tools");
    //this->contextMenuTools->setIcon(QIcon(":/Graphics/Images/New.png"));
    this->contextMenuTools->addMenu(this->contextMenuLibrary);
    this->contextMenuTools->addMenu(this->contextMenuMixer);
    this->contextMenuTools->addMenu(this->contextMenuOther);
    this->contextMenuTools->addMenu(this->contextMenuTriCaster);
    this->contextMenuTools->addMenu(this->contextMenuAtem);

    this->contextMenuColor = new QMenu(this);
    this->contextMenuColor->setTitle("Colorize Item");
    //this->contextMenuColor->setIcon(QIcon(":/Graphics/Images/Color.png"));
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Chocolate");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "DarkKhaki");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "DarkSlateGray");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Maroon");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "MaroonLight");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "OliveDrab");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "RoyalBlue");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "SeaGreen");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Sienna");
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "SteelBlue");
    this->contextMenuColor->addSeparator();
    this->contextMenuColor->addAction(/*QIcon(":/Graphics/Images/Color.png"),*/ "Reset");

    this->contextMenuRundown = new QMenu(this);
    this->contextMenuRundown->addMenu(this->contextMenuTools);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/GroupSmall.png"),*/ "Group");
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/UngroupSmall.png"),*/ "Ungroup");
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
    else if (event.getKey() == Qt::Key_F2) // Play.
        executeCommand(Playout::PlayoutType::Play, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F3) // Load.
        executeCommand(Playout::PlayoutType::Load, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F4) // Pause.
        executeCommand(Playout::PlayoutType::Pause, Action::ActionType::KeyPress);
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
        executeCommand(Playout::PlayoutType::ClearVideolayer, Action::ActionType::KeyPress);
    else if (event.getKey() == Qt::Key_F12) // Clear channel.
        executeCommand(Playout::PlayoutType::ClearChannel, Action::ActionType::KeyPress);
}

void RundownTreeWidget::saveAsPreset(const SaveAsPresetEvent& event)
{
    if (!this->active)
        return;

    saveAsPreset();
}

bool RundownTreeWidget::removeSelectedItems()
{
    this->treeWidgetRundown->removeSelectedItems();
    this->treeWidgetRundown->checkEmptyRundown();

    return true;
}

void RundownTreeWidget::addPresetItem(const AddPresetItemEvent& event)
{
    if (!this->active)
        return;

    qApp->clipboard()->setText(event.getPreset());
    pasteSelectedItems();
    selectItemBelow();

    this->treeWidgetRundown->doItemsLayout(); // Refresh.

    this->treeWidgetRundown->checkEmptyRundown();
}

void RundownTreeWidget::toggleCompactView(const CompactViewEvent& event)
{
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

void RundownTreeWidget::remoteRundownTriggering(const RemoteRundownTriggeringEvent& event)
{
    if (!this->active)
        return;

    this->allowRemoteTriggering = event.getEnabled();

    (this->allowRemoteTriggering == true) ? configureOscSubscriptions() : resetOscSubscriptions();
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

        if (dynamic_cast<VideoCommand*>(childRundownWidget->getCommand()))
            dynamic_cast<VideoCommand*>(childRundownWidget->getCommand())->setAutoPlay(event.getAutoPlay());
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
        this->treeWidgetRundown->setCurrentItem(this->activeItem);

        if (this->treeWidgetRundown->currentItem() != NULL)
        {
            QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
            QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);

            dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->setActive(this->active);
        }
    }
    else
    {
        if (this->treeWidgetRundown->currentItem() != NULL)
        {
            this->activeItem = this->treeWidgetRundown->currentItem();

            QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
            QWidget* currentItemWidget = this->treeWidgetRundown->itemWidget(currentItem, 0);

            dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->setActive(this->active);
        }
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
        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        // Use synchronous event through sendEvent(). Make sure we update the right item in the
        // inspector which will not be the case with postEvent() if we trigger keys really fast.
        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
    }
}

void RundownTreeWidget::openRundown(const QString& path)
{
    QTime time;
    time.start();

    QFile file(path);
    if (file.open(QFile::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        qApp->clipboard()->setText(stream.readAll());
        pasteSelectedItems();

        file.close();

        std::cout << QString("RundownTreeWidget::eventFilter: Parsing rundown file completed, %1 msec").arg(time.elapsed()).toStdString();

        if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
            this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

        this->treeWidgetRundown->setFocus();
    }

    this->treeWidgetRundown->checkEmptyRundown();

    std::cout << QString("%1 msec (%2)").arg(time.elapsed()).arg(this->treeWidgetRundown->invisibleRootItem()->childCount()).toStdString();

    this->activeRundown = path;
}

void RundownTreeWidget::openRundownFromUrl(const QString& path)
{
    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doOpenRundownFromUrl(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(QUrl(path)));
}

void RundownTreeWidget::doOpenRundownFromUrl(QNetworkReply* reply)
{
    qApp->clipboard()->setText(QString::fromUtf8(reply->readAll()));
    pasteSelectedItems();

    if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
        this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

    this->treeWidgetRundown->setFocus();

    this->treeWidgetRundown->checkEmptyRundown();

    this->activeRundown = reply->request().url().toString();
}

void RundownTreeWidget::saveRundown(bool saveAs)
{
    if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
        return;

    QString path;
    if (saveAs)
        path = QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)");
    else
        path = (this->activeRundown == Rundown::DEFAULT_NAME) ? QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)") : this->activeRundown;

    if (!path.isEmpty())
    {
        EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Saving rundown..."));

        QFile file(path);
        if (file.exists())
            file.remove();

        if (file.open(QFile::WriteOnly))
        {
            QXmlStreamWriter* writer = new QXmlStreamWriter();
            writer->setDevice(&file);

            writer->writeStartDocument();
            writer->writeStartElement("items");
            writer->writeTextElement("allowremotetriggering", (this->allowRemoteTriggering == true) ? "true" : "false");

            for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
                this->treeWidgetRundown->writeProperties(this->treeWidgetRundown->invisibleRootItem()->child(i), writer);

            writer->writeEndElement();
            writer->writeEndDocument();

            file.close();
        }

        this->treeWidgetRundown->checkEmptyRundown();

        this->activeRundown = path;
        EventManager::getInstance().fireActiveRundownChangedEvent(ActiveRundownChangedEvent(this->activeRundown));
        EventManager::getInstance().fireStatusbarEvent(StatusbarEvent(""));
    }
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
    executeCommand(gpiBindings[gpiPort], Action::ActionType::GpiPulse);
}

void RundownTreeWidget::gpiBindingChanged(int gpiPort, Playout::PlayoutType::Type binding)
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
        this->contextMenuRundown->actions().at(5)->setEnabled(false); // Colorize.
        this->contextMenuRundown->actions().at(7)->setEnabled(false); // Save as Preset.
        this->contextMenuRundown->actions().at(9)->setEnabled(false); // Remove.
    }

    this->contextMenuRundown->exec(this->treeWidgetRundown->mapToGlobal(point));
}

void RundownTreeWidget::contextMenuNewTriggered(QAction* action)
{
}

void RundownTreeWidget::contextMenuColorTriggered(QAction* action)
{
    if (action->text() == "Sienna")
        colorizeItems(Color::SIENNA_COLOR);
    else if (action->text() == "OliveDrab")
        colorizeItems(Color::OLIVEDRAB_COLOR);
    else if (action->text() == "SeaGreen")
        colorizeItems(Color::SEAGREEN_COLOR);
    else if (action->text() == "Chocolate")
        colorizeItems(Color::CHOCOLATE_COLOR);
    else if (action->text() == "DarkSlateGray")
        colorizeItems(Color::DARKSLATEGRAY_COLOR);
    else if (action->text() == "SteelBlue")
        colorizeItems(Color::STEELBLUE_COLOR);
    else if (action->text() == "Maroon")
        colorizeItems(Color::MAROON_COLOR);
    else if (action->text() == "MaroonLight")
        colorizeItems(Color::MAROONLIGHT_COLOR);
    else if (action->text() == "DarkKhaki")
        colorizeItems(Color::DARKKHAKI_COLOR);
    else if (action->text() == "RoyalBlue")
        colorizeItems(Color::ROYALBLUE_COLOR);
    else
        colorizeItems(""); // Reset
}

void RundownTreeWidget::contextMenuRundownTriggered(QAction* action)
{
    if (action->text() == "Group")
        this->treeWidgetRundown->groupItems();
    else if (action->text() == "Ungroup")
        this->treeWidgetRundown->ungroupItems();
}

void RundownTreeWidget::itemClicked(QTreeWidgetItem* current, int i)
{
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

        // Use synchronous event through sendEvent(). Make sure we update the right item in the
        // inspector which will not be the case with postEvent() if we trigger keys really fast.
        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
    }
}

void RundownTreeWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    QWidget* currentWidget = this->treeWidgetRundown->itemWidget(current, 0);
    QWidget* previousWidget = this->treeWidgetRundown->itemWidget(previous, 0);

    if (previous != NULL && previousWidget != NULL)
        dynamic_cast<AbstractRundownWidget*>(previousWidget)->setActive(false);

    if (current != NULL && currentWidget != NULL)
        dynamic_cast<AbstractRundownWidget*>(currentWidget)->setActive(true);

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

        // Use synchronous event through sendEvent(). Make sure we update the right item in the
        // inspector which will not be the case with postEvent() if we trigger keys really fast.
        EventManager::getInstance().fireRundownItemSelectedEvent(RundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent));
    }
    else if (currentItem == NULL && previous != NULL && this->treeWidgetRundown->invisibleRootItem()->childCount() == 1) // Last item was removed form the rundown.
    {
        // Use synchronous event through sendEvent(). Make sure we update the right item in the
        // inspector which will not be the case with postEvent() if we trigger keys really fast.
        EventManager::getInstance().fireEmptyRundownEvent(EmptyRundownEvent());
    }
}

void RundownTreeWidget::itemDoubleClicked(QTreeWidgetItem* item, int index)
{
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
    bool result = this->treeWidgetRundown->pasteSelectedItems();

    this->treeWidgetRundown->checkEmptyRundown();

    return result;
}

bool RundownTreeWidget::copySelectedItems() const
{
    return this->treeWidgetRundown->copySelectedItems();
}

bool RundownTreeWidget::executeCommand(Playout::PlayoutType::Type type, Action::ActionType::Type source, QTreeWidgetItem* item)
{
    //QModelIndex currentIndex;
    QTreeWidgetItem* currentItem = NULL;

    QWidget* selectedWidget = NULL;
    QWidget* selectedWidgetParent = NULL;

    AbstractRundownWidget* rundownWidget = NULL;
    AbstractRundownWidget* rundownWidgetParent = NULL;

    if (item == NULL)
    {
        if (this->treeWidgetRundown->currentItem() == NULL)
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

    if (type == Playout::PlayoutType::Next && rundownWidgetParent != NULL && rundownWidgetParent->isGroup() && dynamic_cast<GroupCommand*>(rundownWidgetParent->getCommand())->getAutoPlay())
    {
        EventManager::getInstance().fireAutoPlayNextRundownItemEvent(AutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget)));

        return true;
    }
    else
    {
        dynamic_cast<AbstractPlayoutCommand*>(selectedWidget)->executeCommand(type);
    }

    if (rundownWidget != NULL && rundownWidget->isGroup())
    {
        if (type == Playout::PlayoutType::Next && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
            EventManager::getInstance().fireAutoPlayNextRundownItemEvent(AutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget)));
        else if ((type == Playout::PlayoutType::Play || type == Playout::PlayoutType::Load) && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
        {
            // The group have auto play enabled, then we want to play the items within the group.
            bool isFirstChild = true;

            QList<AbstractRundownWidget*>* autoPlayQueue = new QList<AbstractRundownWidget*>();
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->child(i), 0);
                AbstractRundownWidget* rundownChildWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);
                if (dynamic_cast<VideoCommand*>(rundownChildWidget->getCommand()))
                {
                    // Auto play is set on the group. Skip video items without auto play.
                    if (!dynamic_cast<VideoCommand*>(rundownChildWidget->getCommand())->getAutoPlay())
                        continue;

                    // Only execute the first child in the group, add the rest to the auto play queue.
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
            for (int i = 0; i < currentItem->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->child(i), 0);

                EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(currentItem->child(i)));

                dynamic_cast<AbstractPlayoutCommand*>(childWidget)->executeCommand(type);

                if (type == Playout::PlayoutType::Preview)
                    return true; // We are done.
            }
        }

        // Check if group item have auto step enabled.
        if (dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoStep() &&
            (type != Playout::PlayoutType::Clear && type != Playout::PlayoutType::ClearVideolayer && type != Playout::PlayoutType::ClearChannel))
        { 
            if (type == Playout::PlayoutType::Play)
            {
                if (this->currentPlayingAutoStepItem != NULL)
                {
                    for (int i = 0; i < this->currentPlayingAutoStepItem->childCount(); i++)
                    {
                        QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->currentPlayingAutoStepItem->child(i), 0);
                        dynamic_cast<AbstractRundownWidget*>(childWidget)->clearDelayedCommands();
                    }
                }

                this->currentPlayingAutoStepItem = currentItem;
            }

            QTimer::singleShot(500, this, SLOT(selectItemBelow()));

            bool previewOnAutoStep = (DatabaseManager::getInstance().getConfigurationByName("PreviewOnAutoStep").getValue() == "true") ? true : false;
            if (previewOnAutoStep)
                QTimer::singleShot(600, this, SLOT(executePreview()));
        }
    }
    else if (rundownWidgetParent != NULL && rundownWidgetParent->isGroup())
    {
        // The parent of the selected item is a group. If group have auto play
        // enabled, then we want to play current item and below within the group.
        if (type == Playout::PlayoutType::Play && dynamic_cast<GroupCommand*>(rundownWidgetParent->getCommand())->getAutoPlay())
        {
            QList<AbstractRundownWidget*>* autoPlayQueue = new QList<AbstractRundownWidget*>();
            //for (int i = currentIndex.row(); i < currentItem->parent()->childCount(); i++)
            for (int i = currentItem->parent()->indexOfChild(currentItem); i < currentItem->parent()->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(currentItem->parent()->child(i), 0);
                AbstractRundownWidget* rundownChildWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);
                if (dynamic_cast<VideoCommand*>(rundownChildWidget->getCommand()))
                {
                    // Auto play is set on the group. Skip video items without auto play.
                    if (!dynamic_cast<VideoCommand*>(rundownChildWidget->getCommand())->getAutoPlay())
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

void RundownTreeWidget::addGeometryItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GEOMETRY);
}

void RundownTreeWidget::addGpiOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::GPIOUTPUT);
}

void RundownTreeWidget::addOscOutputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::OSCOUTPUT);
}

void RundownTreeWidget::addFileRecorderItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::FILERECORDER);
}

void RundownTreeWidget::addSeparatorItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::SEPARATOR);
}

void RundownTreeWidget::addTriggerAutoItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERAUTO);
}

void RundownTreeWidget::addTriggerTakeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERTAKE);
}

void RundownTreeWidget::addPlayMacroItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERMACRO);
}

void RundownTreeWidget::addSelectInputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERINPUT);
}

void RundownTreeWidget::addSelectPresetItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERPRESET);
}

void RundownTreeWidget::addSelectNetworkSourceItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TRICASTERNETSOURCE);
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
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::IMAGE);
}

void RundownTreeWidget::addTemplateItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TEMPLATE);
}

void RundownTreeWidget::addVideoItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::VIDEO);
}

void RundownTreeWidget::addAtemKeyerStateItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMKEYERSTATE);
}

void RundownTreeWidget::addAtemSelectInputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMINPUT);
}

void RundownTreeWidget::addAtemTriggerAutoItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMAUTO);
}

void RundownTreeWidget::addAtemTriggerCutItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMCUT);
}

void RundownTreeWidget::addAtemVideoFormatItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMVIDEOFORMAT);
}

void RundownTreeWidget::addAtemAudioInputStateItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMAUDIOINPUTSTATE);
}

void RundownTreeWidget::addAtemAudioInputGainItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMAUDIOGAIN);
}

void RundownTreeWidget::addAtemAudioInputBalanceItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::ATEMAUDIOINPUTBALANCE);
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

bool RundownTreeWidget::getAllowRemoteTriggering() const
{
    return this->allowRemoteTriggering;
}

void RundownTreeWidget::configureOscSubscriptions()
{
    resetOscSubscriptions();

    QFileInfo path(this->activeRundown);

    QString upControlFilter = Osc::DEFAULT_UP_CONTROL_FILTER;
    upControlFilter.replace("#UID#", path.baseName());
    this->upControlSubscription = new OscSubscription(upControlFilter, this);
    QObject::connect(this->upControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(upControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString downControlFilter = Osc::DEFAULT_DOWN_CONTROL_FILTER;
    downControlFilter.replace("#UID#", path.baseName());
    this->downControlSubscription = new OscSubscription(downControlFilter, this);
    QObject::connect(this->downControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(downControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString stopControlFilter = Osc::DEFAULT_STOP_CONTROL_FILTER;
    stopControlFilter.replace("#UID#", path.baseName());
    this->stopControlSubscription = new OscSubscription(stopControlFilter, this);
    QObject::connect(this->stopControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(stopControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString playControlFilter = Osc::DEFAULT_PLAY_CONTROL_FILTER;
    playControlFilter.replace("#UID#", path.baseName());
    this->playControlSubscription = new OscSubscription(playControlFilter, this);
    QObject::connect(this->playControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(playControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString loadControlFilter = Osc::DEFAULT_LOAD_CONTROL_FILTER;
    loadControlFilter.replace("#UID#", path.baseName());
    this->loadControlSubscription = new OscSubscription(loadControlFilter, this);
    QObject::connect(this->loadControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(loadControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString nextControlFilter = Osc::DEFAULT_NEXT_CONTROL_FILTER;
    nextControlFilter.replace("#UID#", path.baseName());
    this->nextControlSubscription = new OscSubscription(nextControlFilter, this);
    QObject::connect(this->nextControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(nextControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString updateControlFilter = Osc::DEFAULT_UPDATE_CONTROL_FILTER;
    updateControlFilter.replace("#UID#", path.baseName());
    this->updateControlSubscription = new OscSubscription(updateControlFilter, this);
    QObject::connect(this->updateControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(updateControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString invokeControlFilter = Osc::DEFAULT_INVOKE_CONTROL_FILTER;
    invokeControlFilter.replace("#UID#", path.baseName());
    this->invokeControlSubscription = new OscSubscription(invokeControlFilter, this);
    QObject::connect(this->invokeControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearControlFilter = Osc::DEFAULT_CLEAR_CONTROL_FILTER;
    clearControlFilter.replace("#UID#", path.baseName());
    this->clearControlSubscription = new OscSubscription(clearControlFilter, this);
    QObject::connect(this->clearControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearVideolayerControlFilter = Osc::DEFAULT_CLEAR_VIDEOLAYER_CONTROL_FILTER;
    clearVideolayerControlFilter.replace("#UID#", path.baseName());
    this->clearVideolayerControlSubscription = new OscSubscription(clearVideolayerControlFilter, this);
    QObject::connect(this->clearVideolayerControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&)));

    QString clearChannelControlFilter = Osc::DEFAULT_CLEAR_CHANNEL_CONTROL_FILTER;
    clearChannelControlFilter.replace("#UID#", path.baseName());
    this->clearChannelControlSubscription = new OscSubscription(clearChannelControlFilter, this);
    QObject::connect(this->clearChannelControlSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void RundownTreeWidget::resetOscSubscriptions()
{
    if (this->upControlSubscription != NULL)
        this->upControlSubscription->disconnect(); // Disconnect all events.

    if (this->downControlSubscription != NULL)
        this->downControlSubscription->disconnect(); // Disconnect all events.

    if (this->stopControlSubscription != NULL)
        this->stopControlSubscription->disconnect(); // Disconnect all events.

    if (this->playControlSubscription != NULL)
        this->playControlSubscription->disconnect(); // Disconnect all events.

    if (this->loadControlSubscription != NULL)
        this->loadControlSubscription->disconnect(); // Disconnect all events.

    if (this->pauseControlSubscription != NULL)
        this->pauseControlSubscription->disconnect(); // Disconnect all events.

    if (this->nextControlSubscription != NULL)
        this->nextControlSubscription->disconnect(); // Disconnect all events.

    if (this->updateControlSubscription != NULL)
        this->updateControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearControlSubscription != NULL)
        this->clearControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearVideolayerControlSubscription != NULL)
        this->clearVideolayerControlSubscription->disconnect(); // Disconnect all events.

    if (this->clearChannelControlSubscription != NULL)
        this->clearChannelControlSubscription->disconnect(); // Disconnect all events.
}

void RundownTreeWidget::upControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
    {
        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemAbove();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::downControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
    {
        this->treeWidgetRundown->blockSignals(true);
        this->treeWidgetRundown->selectItemBelow();
        this->treeWidgetRundown->blockSignals(false);
    }
}

void RundownTreeWidget::stopControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Stop, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::playControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Play, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::loadControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Load, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::pauseControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Pause, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::nextControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Next, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::updateControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Update, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::invokeControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Invoke, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::Clear, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearVideolayerControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::ClearVideolayer, this->treeWidgetRundown->currentItem()));
}

void RundownTreeWidget::clearChannelControlSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    if (this->allowRemoteTriggering && arguments.count() > 0 && arguments[0] == 1)
        EventManager::getInstance().fireExecuteRundownItemEvent(ExecuteRundownItemEvent(Playout::PlayoutType::ClearChannel, this->treeWidgetRundown->currentItem()));
}
