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
#include "Events/Rundown/AddRudnownItemEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"
#include "Events/Rundown/AutoPlayNextRundownItemEvent.h"
#include "Events/Rundown/ExecuteRundownItemEvent.h"
#include "Events/AddPresetItemEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
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
      activeRundown(Rundown::DEFAULT_NAME), active(false), enterPressed(false), currentAutoPlayWidget(NULL)
{
    setupUi(this);
    setupMenus();

    // TODO: Specific Gpi device.
    QObject::connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(gpiTriggered(int, GpiDevice*)), this, SLOT(gpiPortTriggered(int, GpiDevice*)));
    QObject::connect(this->treeWidgetRundown, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    foreach (const GpiPortModel& port, DatabaseManager::getInstance().getGpiPorts())
        gpiBindingChanged(port.getPort(), port.getAction());

    checkEmptyRundown();

    qApp->installEventFilter(this);
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

    this->contextMenuTools = new QMenu(this);
    this->contextMenuTools->setTitle("Tools");
    //this->contextMenuTools->setIcon(QIcon(":/Graphics/Images/New.png"));
    this->contextMenuTools->addMenu(this->contextMenuLibrary);
    this->contextMenuTools->addMenu(this->contextMenuMixer);
    this->contextMenuTools->addMenu(this->contextMenuOther);
    this->contextMenuTools->addMenu(this->contextMenuTriCaster);

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
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/PresetSmall.png"),*/ "Save as Preset", this, SLOT(saveAsPreset()));
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/Remove.png"),*/ "Remove", this, SLOT(removeSelectedItems()));

    QObject::connect(this->contextMenuTools, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuNewTriggered(QAction*)));
    QObject::connect(this->contextMenuColor, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuColorTriggered(QAction*)));
    QObject::connect(this->contextMenuRundown, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuRundownTriggered(QAction*)));
}

bool RundownTreeWidget::eventFilter(QObject* target, QEvent* event)
{
    //qDebug() << "Target: " << target << ", Parent: " << target->parent() << ", Event: " << event;

    if (event->type() == static_cast<QEvent::Type>(Event::EventType::ExecuteRundownItem))
    {
        ExecuteRundownItemEvent* executeRundownItemEvent = dynamic_cast<ExecuteRundownItemEvent*>(event);
        if (executeRundownItemEvent->getItem()->treeWidget() == this->treeWidgetRundown)
        {
            executeCommand(executeRundownItemEvent->getType(), KeyPress, executeRundownItemEvent->getItem());

            return true;
        }
    }

    if (this->active)
    {
        qDebug() << "Target: " << target << ", Parent: " << target->parent() << ", Event: " << event;

        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_F1) // Stop.
                return executeCommand(Playout::PlayoutType::Stop, KeyPress);
            else if (keyEvent->key() == Qt::Key_F2) // Play.
                return executeCommand(Playout::PlayoutType::Play, KeyPress);
            else if (keyEvent->key() == Qt::Key_F3) // Load.
                return executeCommand(Playout::PlayoutType::Load, KeyPress);
            else if (keyEvent->key() == Qt::Key_F4) // Pause.
                return executeCommand(Playout::PlayoutType::Pause, KeyPress);
            else if (keyEvent->key() == Qt::Key_F5) // Next.
                return executeCommand(Playout::PlayoutType::Next, KeyPress);
            else if (keyEvent->key() == Qt::Key_F6) // Update.
                return executeCommand(Playout::PlayoutType::Update, KeyPress);
            else if (keyEvent->key() == Qt::Key_F7) // Invoke.
                return executeCommand(Playout::PlayoutType::Invoke, KeyPress);
            else if (keyEvent->key() == Qt::Key_F10) // Clear.
                return executeCommand(Playout::PlayoutType::Clear, KeyPress);
            else if (keyEvent->key() == Qt::Key_F11) // Clear videolayer.
                return executeCommand(Playout::PlayoutType::ClearVideolayer, KeyPress);
            else if (keyEvent->key() == Qt::Key_F12) // Clear channel.
                return executeCommand(Playout::PlayoutType::ClearChannel, KeyPress);

            if (target == this->treeWidgetRundown)
            {
                if (keyEvent->key() == Qt::Key_Delete)
                    return removeSelectedItems();
                else if (keyEvent->key() == Qt::Key_D && keyEvent->modifiers() == Qt::ControlModifier)
                    return duplicateSelectedItems();
                else if (keyEvent->key() == Qt::Key_C && keyEvent->modifiers() == Qt::ControlModifier)
                    return copySelectedItems();
                else if (keyEvent->key() == Qt::Key_V && keyEvent->modifiers() == Qt::ControlModifier)
                    return pasteSelectedItems();
                else if (keyEvent->key() == Qt::Key_G && keyEvent->modifiers() == Qt::ControlModifier)
                    return groupItems();
                else if (keyEvent->key() == Qt::Key_U && keyEvent->modifiers() == Qt::ControlModifier)
                    return ungroupItems();
                else if (keyEvent->key() == Qt::Key_X && keyEvent->modifiers() == Qt::ControlModifier)
                {
                    copySelectedItems();
                    removeSelectedItems();

                    return true;
                }
                else if (keyEvent->key() == Qt::Key_Up && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemUp();
                else if (keyEvent->key() == Qt::Key_Down && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemDown();
                else if (keyEvent->key() == Qt::Key_Left && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemOutOfGroup();
                else if (keyEvent->key() == Qt::Key_Right && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemIntoGroup();
            }
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::ToggleCompactView))
        {
            if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
                return true;

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

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AddRudnownItem))
        {
            AddRudnownItemEvent* addRudnownItemEvent = dynamic_cast<AddRudnownItemEvent*>(event);

            AbstractRundownWidget* widget = RundownItemFactory::getInstance().createWidget(addRudnownItemEvent->getLibraryModel());
            if (widget == NULL)
                return true;

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

            checkEmptyRundown();

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AddPresetItem))
        {
            AddPresetItemEvent* addPresetItemEvent = dynamic_cast<AddPresetItemEvent*>(event);

            qApp->clipboard()->setText(addPresetItemEvent->getPreset());
            pasteSelectedItems();
            selectItemBelow();

            this->treeWidgetRundown->doItemsLayout(); // Refresh.

            checkEmptyRundown();

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AutoPlayRundownItem))
        {
            AutoPlayRundownItemEvent* autoPlayRundownItemEvent = dynamic_cast<AutoPlayRundownItemEvent*>(event);
            AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(autoPlayRundownItemEvent->getSource());
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

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AutoPlayNextRundownItem))
        {
            AutoPlayNextRundownItemEvent* autoPlayNextRundownItemEvent = dynamic_cast<AutoPlayNextRundownItemEvent*>(event);
            AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(autoPlayNextRundownItemEvent->getSource());

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
                        dynamic_cast<AbstractPlayoutCommand*>(rundownQueueWidget)->executeCommand(Playout::PlayoutType::Next);

                        this->currentAutoPlayWidget = rundownQueueWidget;
                    }
                    else
                        this->autoPlayQueues.removeOne(autoPlayQueue);

                    break;
                }
            }

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Event::EventType::AutoPlayChanged))
        {
            AutoPlayChangedEvent* autoPlayChangedEvent = dynamic_cast<AutoPlayChangedEvent*>(event);
            for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
            {
                QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem()->child(i), 0);
                AbstractRundownWidget* childRundownWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);

                if (dynamic_cast<VideoCommand*>(childRundownWidget->getCommand()))
                    dynamic_cast<VideoCommand*>(childRundownWidget->getCommand())->setAutoPlay(autoPlayChangedEvent->getAutoPlay());
            }

            return true;
        }
    }

    return QObject::eventFilter(target, event);
}

void RundownTreeWidget::setActive(bool active)
{
    this->active = active;

    if (!this->active)
    {
        // Deselect / Deactivate current selected item. We do not want to update wrong item.
        this->treeWidgetRundown->setCurrentItem(NULL);

        EventManager::getInstance().fireEmptyRundownEvent(); // Reset inspector panel.
    }

    EventManager::getInstance().fireActiveRundownChangedEvent(this->activeRundown);

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
        EventManager::getInstance().fireRundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent);
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

        qDebug() << QString("RundownTreeWidget::eventFilter: Parsing rundown file completed, %1 msec").arg(time.elapsed());

        if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
            this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

        this->treeWidgetRundown->setFocus();
    }

    checkEmptyRundown();

    qDebug() << QString("%1 msec (%2)").arg(time.elapsed()).arg(this->treeWidgetRundown->invisibleRootItem()->childCount());

    this->activeRundown = path;
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
        EventManager::getInstance().fireStatusbarEvent("Saving rundown...");
        EventManager::getInstance().fireProcessEvent();

        QFile file(path);
        if (file.exists())
            file.remove();

        if (file.open(QFile::WriteOnly))
        {
            QXmlStreamWriter* writer = new QXmlStreamWriter();
            writer->setDevice(&file);

            writer->writeStartDocument();
            writer->writeStartElement("items");
            for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
                this->treeWidgetRundown->writeProperties(this->treeWidgetRundown->invisibleRootItem()->child(i), writer);

            writer->writeEndElement();
            writer->writeEndDocument();

            file.close();
        }

        checkEmptyRundown();

        this->activeRundown = path;
        EventManager::getInstance().fireActiveRundownChangedEvent(this->activeRundown);
        EventManager::getInstance().fireStatusbarEvent("");
        EventManager::getInstance().fireProcessEvent();
    }
}

void RundownTreeWidget::checkEmptyRundown()
{
    this->treeWidgetRundown->setStyleSheet((this->treeWidgetRundown->invisibleRootItem()->childCount() == 0) ? "border-color: red;" : "");
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
    executeCommand(gpiBindings[gpiPort], GpiPulse);
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
        colorizeItems(Color::SIENNA_RGBA_COLOR);
    else if (action->text() == "OliveDrab")
        colorizeItems(Color::OLIVEDRAB_RGBA_COLOR);
    else if (action->text() == "SeaGreen")
        colorizeItems(Color::SEAGREEN_RGBA_COLOR);
    else if (action->text() == "Chocolate")
        colorizeItems(Color::CHOCOLATE_RGBA_COLOR);
    else if (action->text() == "DarkSlateGray")
        colorizeItems(Color::DARKSLATEGRAY_RGBA_COLOR);
    else if (action->text() == "SteelBlue")
        colorizeItems(Color::STEELBLUE_RGBA_COLOR);
    else if (action->text() == "Maroon")
        colorizeItems(Color::MAROON_RGBA_COLOR);
    else if (action->text() == "MaroonLight")
        colorizeItems(Color::MAROONLIGHT_RGBA_COLOR);
    else if (action->text() == "DarkKhaki")
        colorizeItems(Color::DARKKHAKI_RGBA_COLOR);
    else if (action->text() == "RoyalBlue")
        colorizeItems(Color::ROYALBLUE_RGBA_COLOR);
    else
        colorizeItems(""); // Reset
}

void RundownTreeWidget::contextMenuRundownTriggered(QAction* action)
{
    if (action->text() == "Group")
        groupItems();
    else if (action->text() == "Ungroup")
        ungroupItems();
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
        EventManager::getInstance().fireRundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent);
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
        EventManager::getInstance().fireRundownItemSelectedEvent(command, model, currentItemWidget, currentItemWidgetParent);
    }
    else if (currentItem == NULL && previous != NULL && this->treeWidgetRundown->invisibleRootItem()->childCount() == 1) // Last item was removed form the rundown.
    {
        // Use synchronous event through sendEvent(). Make sure we update the right item in the
        // inspector which will not be the case with postEvent() if we trigger keys really fast.
        EventManager::getInstance().fireEmptyRundownEvent();
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

    checkEmptyRundown();

    return result;
}

bool RundownTreeWidget::copySelectedItems() const
{
    return this->treeWidgetRundown->copySelectedItems();
}

bool RundownTreeWidget::moveItemDown()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return true;

    int row  = this->treeWidgetRundown->currentIndex().row();
    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QTreeWidgetItem* parentItem = this->treeWidgetRundown->currentItem()->parent();

    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->isGroup())
    {
        int rowCount = 0;
        if (parentItem == NULL) // Top level item.
            rowCount = this->treeWidgetRundown->invisibleRootItem()->childCount() - 1;

        if (currentItem != NULL && row < rowCount)
        {
            AbstractRundownWidget* parentWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();
            parentWidget->setInGroup(true);
            parentWidget->setExpanded(true);

            QTreeWidgetItem* parentItem = new QTreeWidgetItem();
            this->treeWidgetRundown->invisibleRootItem()->insertChild(row + 2, parentItem);
            this->treeWidgetRundown->setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

            if (this->treeWidgetRundown->currentItem()->isExpanded())
                this->treeWidgetRundown->expandItem(parentItem);

            for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
            {
                QTreeWidgetItem* item = this->treeWidgetRundown->currentItem()->child(i);

                AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();
                childWidget->setInGroup(true);

                QTreeWidgetItem* childItem = new QTreeWidgetItem();
                parentItem->addChild(childItem);
                this->treeWidgetRundown->setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
            }

            delete currentItem;

            this->treeWidgetRundown->setCurrentItem(parentItem);
            this->treeWidgetRundown->doItemsLayout(); // Refresh
        }
    }
    else
    {
        int rowCount = 0;
        if (parentItem == NULL) // Top level item.
            rowCount = this->treeWidgetRundown->invisibleRootItem()->childCount() - 1;
        else
            rowCount = this->treeWidgetRundown->currentItem()->parent()->childCount() - 1;

        if (currentItem != NULL && row < rowCount)
        {
            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();

            if (parentItem == NULL) // Top level item.
            {
                this->treeWidgetRundown->invisibleRootItem()->takeChild(row);
                this->treeWidgetRundown->invisibleRootItem()->insertChild(row + 1, currentItem);
            }
            else // Group item.
            {
                newWidget->setInGroup(true);

                this->treeWidgetRundown->currentItem()->parent()->takeChild(row);
                this->treeWidgetRundown->currentItem()->parent()->insertChild(row + 1, currentItem);
            }

            this->treeWidgetRundown->setItemWidget(currentItem, 0, dynamic_cast<QWidget*>(newWidget));
            this->treeWidgetRundown->setCurrentItem(currentItem);
            this->treeWidgetRundown->doItemsLayout(); // Refresh
        }
    }

    return true;
}

bool RundownTreeWidget::moveItemUp()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return true;

    int row  = this->treeWidgetRundown->currentIndex().row();
    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QTreeWidgetItem* parentItem = this->treeWidgetRundown->currentItem()->parent();

    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->isGroup())
    {
        int rowCount = 0;
        if (currentItem != NULL && row > rowCount)
        {
            AbstractRundownWidget* parentWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();
            parentWidget->setInGroup(true);
            parentWidget->setExpanded(true);

            QTreeWidgetItem* parentItem = new QTreeWidgetItem();
            this->treeWidgetRundown->invisibleRootItem()->insertChild(row - 1, parentItem);
            this->treeWidgetRundown->setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

            if (this->treeWidgetRundown->currentItem()->isExpanded())
                this->treeWidgetRundown->expandItem(parentItem);

            for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
            {
                QTreeWidgetItem* item = this->treeWidgetRundown->currentItem()->child(i);

                AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();
                childWidget->setInGroup(true);

                QTreeWidgetItem* childItem = new QTreeWidgetItem();
                parentItem->addChild(childItem);
                this->treeWidgetRundown->setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
            }

            delete currentItem;

            this->treeWidgetRundown->setCurrentItem(parentItem);
            this->treeWidgetRundown->doItemsLayout(); // Refresh
        }
    }
    else
    {
        int rowCount = 0;
        if (currentItem != NULL && row > rowCount)
        {
            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();

            if (parentItem == NULL) // Top level item.
            {
                this->treeWidgetRundown->invisibleRootItem()->takeChild(row);
                this->treeWidgetRundown->invisibleRootItem()->insertChild(row - 1, currentItem);
            }
            else // Group item.
            {
                newWidget->setInGroup(true);

                this->treeWidgetRundown->currentItem()->parent()->takeChild(row);
                this->treeWidgetRundown->currentItem()->parent()->insertChild(row - 1, currentItem);
            }

            this->treeWidgetRundown->setItemWidget(currentItem, 0, dynamic_cast<QWidget*>(newWidget));
            this->treeWidgetRundown->setCurrentItem(currentItem);
            this->treeWidgetRundown->doItemsLayout(); // Refresh
        }
    }

    return true;
}

bool RundownTreeWidget::executeCommand(Playout::PlayoutType::Type type, ActionSource source, QTreeWidgetItem* item)
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

    if (source == GpiPulse && !rundownWidget->getCommand()->getAllowGpi())
        return true; // Gpi pulses cannot trigger this item.

    if (type == Playout::PlayoutType::Next && rundownWidgetParent != NULL && rundownWidgetParent->isGroup() && dynamic_cast<GroupCommand*>(rundownWidgetParent->getCommand())->getAutoPlay())
    {
        EventManager::getInstance().fireAutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget));

        return true;
    }
    else
        dynamic_cast<AbstractPlayoutCommand*>(selectedWidget)->executeCommand(type);

    if (rundownWidget != NULL && rundownWidget->isGroup())
    {
        if (type == Playout::PlayoutType::Next && dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoPlay())
            EventManager::getInstance().fireAutoPlayNextRundownItemEvent(dynamic_cast<QWidget*>(this->currentAutoPlayWidget));
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
                AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(childWidget);

                if (dynamic_cast<VideoCommand*>(rundownWidget->getCommand()))
                {
                    // Remove item from auto play queue if it exists.
                    foreach (QList<AbstractRundownWidget*>* autoPlayQueue, this->autoPlayQueues)
                    {
                        if (autoPlayQueue->contains(rundownWidget))
                        {
                            autoPlayQueue->removeOne(rundownWidget);

                            if (autoPlayQueue->isEmpty())
                                this->autoPlayQueues.removeOne(autoPlayQueue);

                            break;
                        }
                    }
                }

                dynamic_cast<AbstractPlayoutCommand*>(childWidget)->executeCommand(type);
            }
        }

        // Check if group item have auto step enabled.
        if ((type != Playout::PlayoutType::Clear && type != Playout::PlayoutType::ClearVideolayer && type != Playout::PlayoutType::ClearChannel) &&
            dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoStep())
        {
            QTimer::singleShot(500, this, SLOT(selectItemBelow()));
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

bool RundownTreeWidget::groupItems()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return true;

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

    if (isGroup || isGroupItem)
        return true; // We don't support group in groups.

    QTreeWidgetItem* parentItem = new QTreeWidgetItem();

    RundownGroupWidget* widget = new RundownGroupWidget(LibraryModel(0, "Group", "", "", "GROUP", 0, ""), this->treeWidgetRundown);
    widget->setActive(true);
    widget->setExpanded(true);
    widget->setCompactView(this->treeWidgetRundown->getCompactView());

    int row = this->treeWidgetRundown->indexOfTopLevelItem(this->treeWidgetRundown->selectedItems().at(0));
    this->treeWidgetRundown->invisibleRootItem()->insertChild(row, parentItem);
    this->treeWidgetRundown->setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(widget));
    this->treeWidgetRundown->expandItem(parentItem);

    if (this->treeWidgetRundown->getCompactView())
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);

    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
    {
        QTreeWidgetItem* childItem = new QTreeWidgetItem();
        parentItem->addChild(childItem);

        AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();
        childWidget->setInGroup(true);
        childWidget->setActive(false);

        this->treeWidgetRundown->setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
    }

    removeSelectedItems();

    this->treeWidgetRundown->doItemsLayout(); // Refresh
    this->treeWidgetRundown->setCurrentItem(parentItem);

    return true;
}

bool RundownTreeWidget::ungroupItems()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return true;

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

    if (isTopItem || (isGroup && isGroupItem) || (isTopItem && isGroupItem))
        return true; // We don't have any group to ungroup.

    QTreeWidgetItem* rootItem = this->treeWidgetRundown->invisibleRootItem();

    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0))->isGroup()) // Group.
    {
        QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
        QTreeWidgetItem* currentItemAbove = this->treeWidgetRundown->itemAbove(this->treeWidgetRundown->currentItem());

        int row = this->treeWidgetRundown->indexOfTopLevelItem(this->treeWidgetRundown->selectedItems().at(0));

        QTreeWidgetItem* newItem = NULL;
        for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
        {
            QTreeWidgetItem* item = this->treeWidgetRundown->currentItem()->child(i);

            newItem = new QTreeWidgetItem();
            rootItem->insertChild(row + 1, newItem);

            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();
            newWidget->setInGroup(false);
            newWidget->setActive(false);

            this->treeWidgetRundown->setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));

            row++;
        }

        this->treeWidgetRundown->setCurrentItem(currentItemAbove);

        delete currentItem;
    }
    else // Group item.
    {
        QTreeWidgetItem* parentItem = this->treeWidgetRundown->currentItem()->parent();

        int parentRow = this->treeWidgetRundown->indexOfTopLevelItem(this->treeWidgetRundown->currentItem()->parent());

        QTreeWidgetItem* newItem = NULL;
        foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
        {
            newItem = new QTreeWidgetItem();
            rootItem->insertChild(parentRow + 1, newItem);

            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();
            newWidget->setInGroup(false);
            newWidget->setActive(false);

            this->treeWidgetRundown->setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));

            delete item;

            parentRow++;
        }

        this->treeWidgetRundown->setCurrentItem(newItem);

        if (parentItem->childCount() == 0)
            delete parentItem;
    }

    this->treeWidgetRundown->doItemsLayout(); // Refresh

    return true;
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
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::AUTO);
}

void RundownTreeWidget::addTriggerTakeItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::TAKE);
}

void RundownTreeWidget::addPlayMacroItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::MACRO);
}

void RundownTreeWidget::addSelectInputItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::INPUT);
}

void RundownTreeWidget::addSelectPresetItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::PRESET);
}

void RundownTreeWidget::addSelectNetworkSourceItem()
{
    EventManager::getInstance().fireAddRudnownItemEvent(Rundown::NETSOURCE);
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

bool RundownTreeWidget::moveItemOutOfGroup()
{
    if (this->treeWidgetRundown->currentItem() == NULL || this->treeWidgetRundown->currentItem()->parent() == NULL) // Top level item.
        return true;

    QTreeWidgetItem* newItem = new QTreeWidgetItem();
    QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();
    QTreeWidgetItem* parentItem = this->treeWidgetRundown->currentItem()->parent(); // Group.
    QTreeWidgetItem* currentItemAbove = this->treeWidgetRundown->itemAbove(this->treeWidgetRundown->currentItem());
    QTreeWidgetItem* parentItemAbove = this->treeWidgetRundown->itemAbove(this->treeWidgetRundown->currentItem()->parent());

    int currentRow  = this->treeWidgetRundown->currentIndex().row();
    int parentRow  = this->treeWidgetRundown->indexOfTopLevelItem(this->treeWidgetRundown->currentItem()->parent());

    AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();
    newWidget->setInGroup(false);

    this->treeWidgetRundown->currentItem()->parent()->takeChild(currentRow);
    this->treeWidgetRundown->invisibleRootItem()->insertChild(parentRow + 1, newItem);
    this->treeWidgetRundown->setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));
    this->treeWidgetRundown->setCurrentItem(newItem);
    this->treeWidgetRundown->doItemsLayout(); // Refresh

    delete currentItem;

    if (parentItem->childCount() == 0)
    {
        this->treeWidgetRundown->setCurrentItem(parentItemAbove);
        delete parentItem;
    }

    return true;
}

bool RundownTreeWidget::moveItemIntoGroup()
{
    if (this->treeWidgetRundown->currentItem() == NULL || this->treeWidgetRundown->currentItem()->parent() != NULL) // Group item.
        return true;

    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0))->isGroup())
        return true;

    QTreeWidgetItem* currentItemAbove = this->treeWidgetRundown->invisibleRootItem()->child(this->treeWidgetRundown->currentIndex().row() - 1);
    if (currentItemAbove != NULL && dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItemAbove, 0))->isGroup()) // Group.
    {
        QTreeWidgetItem* newItem = new QTreeWidgetItem();
        QTreeWidgetItem* currentItem = this->treeWidgetRundown->currentItem();

        int currentRow  = this->treeWidgetRundown->currentIndex().row();

        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(currentItem, 0))->clone();
        widget->setInGroup(true);

        currentItemAbove->addChild(newItem);

        this->treeWidgetRundown->invisibleRootItem()->takeChild(currentRow);
        this->treeWidgetRundown->setItemWidget(newItem, 0, dynamic_cast<QWidget*>(widget));
        this->treeWidgetRundown->doItemsLayout(); // Ref resh
        this->treeWidgetRundown->setCurrentItem(newItem);

        delete currentItem;
    }

    return true;
}

void RundownTreeWidget::saveAsPreset()
{
    if (!copySelectedItems())
        return;

    PresetDialog* dialog = new PresetDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        DatabaseManager::getInstance().insertPreset(PresetModel(0, dialog->getName(), qApp->clipboard()->text()));
        EventManager::getInstance().firePresetChangedEvent();
    }
}

bool RundownTreeWidget::removeSelectedItems()
{
    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
    {
        // Remove our items from the auto play queue if they exists.
        AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0));
        if (dynamic_cast<VideoCommand*>(rundownWidget->getCommand()))
        {
            foreach (QList<AbstractRundownWidget*>* autoPlayQueue, this->autoPlayQueues)
            {
                if (autoPlayQueue->contains(rundownWidget))
                {
                    autoPlayQueue->removeOne(rundownWidget);

                    if (autoPlayQueue->isEmpty())
                        this->autoPlayQueues.removeOne(autoPlayQueue);

                    break;
                }
            }
        }

        delete rundownWidget;
        delete item;
    }

    checkEmptyRundown();

    return true;
}
