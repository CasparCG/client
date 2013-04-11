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

#include "GpiManager.h"
#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/AddRudnownItemEvent.h"
#include "Events/OpenRundownEvent.h"
#include "Events/SaveRundownEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/RundownModel.h"

#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QTextCodec>
#include <QtCore/QTime>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidgetItem>

RundownTreeWidget::RundownTreeWidget(QWidget* parent)
    : QWidget(parent),
      currentFilename(""), active(false), compactView(false), enterPressed(false)
{
    setupUi(this);

    setupMenus();

    this->rundownAnimation = new BorderAnimation(this->treeWidgetRundown);

    // TODO: specific Gpi device.
    connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(gpiTriggered(int, GpiDevice*)), this, SLOT(gpiPortTriggered(int, GpiDevice*)));
    connect(this->treeWidgetRundown, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    foreach (const GpiPortModel& port, DatabaseManager::getInstance().getGpiPorts())
        gpiBindingChanged(port.getPort(), port.getAction());

    checkEmptyRundown();

    qApp->installEventFilter(this);
}

RundownTreeWidget::~RundownTreeWidget()
{
    this->rundownAnimation->stop();
}

void RundownTreeWidget::setupMenus()
{
    this->contextMenuMixer = new QMenu(this);
    this->contextMenuMixer->setTitle("Mixer");
    //this->contextMenuMixer->setIcon(QIcon(":/Graphics/Images/Mixer.png"));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BlendModeSmall.png"), "Blend Mode", this, SLOT(addBlendModeItem()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BrightnessSmall.png"), "Brightness", this, SLOT(addBrightnessItem()));
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

    this->contextMenuNew = new QMenu(this);
    this->contextMenuNew->setTitle("New");
    //this->contextMenuNew->setIcon(QIcon(":/Graphics/Images/New.png"));
    this->contextMenuNew->addMenu(this->contextMenuLibrary);
    this->contextMenuNew->addMenu(this->contextMenuMixer);
    this->contextMenuNew->addSeparator();
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SolidColorSmall.png"), "Solid Color", this, SLOT(addSolidColorItem()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/GpiOutputSmall.png"), "GPI Output", this, SLOT(addGpiOutputItem()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/FileRecorderSmall.png"), "File Recorder", this, SLOT(addFileRecorderItem()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/DeckLinkProducerSmall.png"), "DeckLink Input", this, SLOT(addDeckLinkInputItem()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SnapshotSmall.png"), "Channel Snapshot", this, SLOT(addPrintItem()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/ClearSmall.png"), "Clear Output", this, SLOT(addClearOutputItem()));
    this->contextMenuNew->addSeparator();
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SeparatorSmall.png"), "Separator", this, SLOT(addSeparatorItem()));
    //this->contextMenuNew->actions().at(3)->setEnabled(false);

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
    this->contextMenuRundown->addMenu(this->contextMenuNew);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/GroupSmall.png"),*/ "Group");
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/UngroupSmall.png"),*/ "Ungroup");
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addMenu(this->contextMenuColor);
    this->contextMenuRundown->addSeparator();
    this->contextMenuRundown->addAction(/*QIcon(":/Graphics/Images/Remove.png"),*/ "Remove", this, SLOT(removeSelectedItems()));

    connect(this->contextMenuNew, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuNewTriggered(QAction*)));
    connect(this->contextMenuColor, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuColorTriggered(QAction*)));
    connect(this->contextMenuRundown, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuRundownTriggered(QAction*)));
}

bool RundownTreeWidget::eventFilter(QObject* target, QEvent* event)
{
    if (this->active)
    {
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
            else if (keyEvent->key() == Qt::Key_F6) // Next.
                return executeCommand(Playout::PlayoutType::Next, KeyPress);
            else if (keyEvent->key() == Qt::Key_F7) // Update.
                return executeCommand(Playout::PlayoutType::Update, KeyPress);
            else if (keyEvent->key() == Qt::Key_F8) // Invoke.
                return executeCommand(Playout::PlayoutType::Invoke, KeyPress);
            else if (keyEvent->key() == Qt::Key_F10) // Clear.
                return executeCommand(Playout::PlayoutType::Clear, KeyPress);
            else if (keyEvent->key() == Qt::Key_F11) // Clear videolayer.
                return executeCommand(Playout::PlayoutType::ClearVideolayer, KeyPress);
            else if (keyEvent->key() == Qt::Key_F12) // Clear channel.
                return executeCommand(Playout::PlayoutType::ClearChannel, KeyPress);

            if (target == treeWidgetRundown)
            {
                if (keyEvent->key() == Qt::Key_Delete)
                    return removeSelectedItems();
                else if (keyEvent->key() == Qt::Key_D && keyEvent->modifiers() == Qt::ControlModifier)
                    return duplicateSelectedItem();
                else if (keyEvent->key() == Qt::Key_C && keyEvent->modifiers() == Qt::ControlModifier)
                    return copySelectedItem();
                else if (keyEvent->key() == Qt::Key_V && keyEvent->modifiers() == Qt::ControlModifier)
                    return pasteSelectedItem();
                else if (keyEvent->key() == Qt::Key_G && keyEvent->modifiers() == Qt::ControlModifier)
                    return groupItems();
                else if (keyEvent->key() == Qt::Key_U && keyEvent->modifiers() == Qt::ControlModifier)
                    return ungroupItems();
                else if (keyEvent->key() == Qt::Key_Up && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemUp();
                else if (keyEvent->key() == Qt::Key_Down && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemDown();
                else if (keyEvent->key() == Qt::Key_Left && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemOutOfGroup();
                else if (keyEvent->key() == Qt::Key_Right && (keyEvent->modifiers() == Qt::ControlModifier || (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->modifiers() & Qt::KeypadModifier)))
                    return moveItemIntoGroup();
            }

            /*
            if (target == treeWidgetRundown)
            {
                QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
                parsePage(keyEvent);

                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Plus || keyEvent->key() == Qt::Key_Minus)
                    playSelectedEvent();
            }
            */
        }
        else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::OpenRundown))
        {
            QString filename = QFileDialog::getOpenFileName(this, "Open Rundown", "", "Rundown (*.xml)");
            if (!filename.isEmpty())
            {
                EventManager::getInstance().fireStatusbarEvent("Opening rundown...");

                QTime time;
                time.start();

                QFile file(filename);
                if (file.open(QFile::ReadOnly | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream.setCodec(QTextCodec::codecForName("UTF-8"));

                    std::wstringstream wstringstream;
                    wstringstream << stream.readAll().toStdWString();

                    file.close();

                    this->treeWidgetRundown->clear();

                    boost::property_tree::wptree pt;
                    boost::property_tree::xml_parser::read_xml(wstringstream, pt);
                    BOOST_FOREACH(boost::property_tree::wptree::value_type& value, pt.get_child(L"items"))
                    {
                        QString type = QString::fromStdWString(value.second.get<std::wstring>(L"type")).toUpper();

                        if (type == "GROUP")
                            readRundownGroup(type, value.second);
                        else
                            readRundownItem(type, value.second, NULL);
                    }

                    qDebug() << QString("RundownTreeWidget::eventFilter: Parsing rundown file completed, %1 msec").arg(time.elapsed());

                    if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
                        this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

                    this->treeWidgetRundown->setFocus();
                }

                checkEmptyRundown();

                qDebug() << QString("%1 msec (%2)").arg(time.elapsed()).arg(this->treeWidgetRundown->invisibleRootItem()->childCount());

                this->currentFilename = filename;
                EventManager::getInstance().fireWindowTitleEvent(this->currentFilename);
            }

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::SaveRundown))
        {
            if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
                return false;

            QString filename;
            SaveRundownEvent* saveRundownEvent = dynamic_cast<SaveRundownEvent*>(event);
            if (saveRundownEvent->getSaveAs())
                filename = QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)");
            else
                filename = (!this->currentFilename.isEmpty()) ? this->currentFilename : QFileDialog::getSaveFileName(this, "Save Rundown", "", "Rundown (*.xml)");

            if (!filename.isEmpty())
            {
                EventManager::getInstance().fireStatusbarEvent("Saving rundown...");

                QFile file(filename);
                if (file.exists())
                    file.remove();

                if (file.open(QFile::WriteOnly))
                {
                    QXmlStreamWriter* writer = new QXmlStreamWriter();
                    writer->setDevice(&file);

                    writer->writeStartDocument();

                    writer->writeStartElement("items");
                    for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
                    {
                        QTreeWidgetItem* child = this->treeWidgetRundown->invisibleRootItem()->child(i);
                        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(child, 0));

                        QString type = widget->getLibraryModel()->getType().toUpper();

                        if (type == "GROUP")
                            writeRundownGroup(type, writer, child);
                        else
                            writeRundownItem(type, writer, child);
                    }
                    writer->writeEndElement();

                    writer->writeEndDocument();
                    delete writer;

                    file.close();
                }

                checkEmptyRundown();

                this->currentFilename = filename;
                EventManager::getInstance().fireWindowTitleEvent(this->currentFilename);
            }

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::ToggleCompactView))
        {
            if (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0)
                return false;

            for (int i = 0; i < this->treeWidgetRundown->invisibleRootItem()->childCount(); i++)
            {
                QTreeWidgetItem* item = this->treeWidgetRundown->invisibleRootItem()->child(i);
                QWidget* widget = dynamic_cast<QWidget*>(this->treeWidgetRundown->itemWidget(item, 0));

                dynamic_cast<AbstractRundownWidget*>(widget)->setCompactView(!this->compactView);
                if (this->compactView)
                    widget->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);
                else
                    widget->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);

                for (int j = 0; j < item->childCount(); j++)
                {
                    QTreeWidgetItem* child = item->child(j);
                    QWidget* widget = dynamic_cast<QWidget*>(this->treeWidgetRundown->itemWidget(child, 0));

                    dynamic_cast<AbstractRundownWidget*>(widget)->setCompactView(!this->compactView);
                    if (this->compactView)
                        widget->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);
                    else
                        widget->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);
                }
            }

            this->treeWidgetRundown->doItemsLayout(); // Refresh

            this->compactView = !this->compactView;

            return true;
        }
        else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::AddRudnownItem))
        {
            AddRudnownItemEvent* addRudnownItemEvent = dynamic_cast<AddRudnownItemEvent*>(event);

            AbstractRundownWidget* widget = RundownItemFactory::getInstance().createWidget(addRudnownItemEvent->getLibraryModel());
            widget->setCompactView(this->compactView);

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

            if (this->compactView)
                dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);
            else
                dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);

            this->treeWidgetRundown->doItemsLayout(); // Refresh.

            checkEmptyRundown();

            return true;
        }
    }

    return QObject::eventFilter(target, event);
}

void RundownTreeWidget::setActive(bool active)
{
    this->active = active;
}

void RundownTreeWidget::readRundownGroup(const QString& type, boost::property_tree::wptree& pt)
{
    QString label = QString::fromStdWString(pt.get<std::wstring>(L"label"));
    bool expanded = pt.get<bool>(L"expanded");

    AbstractRundownWidget* widget = new RundownGroupWidget(LibraryModel(0, label, "", "", type, 0), this);
    widget->setExpanded(true);
    widget->setCompactView(this->compactView);
    widget->getCommand()->readProperties(pt);
    widget->readProperties(pt);

    QTreeWidgetItem* parent = new QTreeWidgetItem();
    this->treeWidgetRundown->invisibleRootItem()->addChild(parent);
    this->treeWidgetRundown->setItemWidget(parent, 0, dynamic_cast<QWidget*>(widget));
    this->treeWidgetRundown->expandItem(parent);

    if (this->compactView)
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);

    this->treeWidgetRundown->doItemsLayout(); // Refresh.

    parent->setExpanded(expanded);

    BOOST_FOREACH(boost::property_tree::wptree::value_type& value, pt.get_child(L"items"))
    {
        QString type = QString::fromStdWString(value.second.get<std::wstring>(L"type")).toUpper();

        readRundownItem(type, value.second, parent);
    }
}

void RundownTreeWidget::writeRundownGroup(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item)
{
    AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0));

    QString deviceName = widget->getLibraryModel()->getDeviceName();
    QString label = widget->getLibraryModel()->getLabel();
    QString name = widget->getLibraryModel()->getName();

    writer->writeStartElement("item");

    writer->writeTextElement("type", type);
    writer->writeTextElement("label", label);
    writer->writeTextElement("expanded", (item->isExpanded() == true ? "true" : "false"));

    widget->getCommand()->writeProperties(writer);
    widget->writeProperties(writer);

    writer->writeStartElement("items");
    for (int i = 0; i < item->childCount(); i++)
    {
        QTreeWidgetItem* child = item->child(i);
        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(child, 0));

        QString type = widget->getLibraryModel()->getType().toUpper();

        writeRundownItem(type, writer, child);
    }
    writer->writeEndElement();

    writer->writeEndElement();
}

void RundownTreeWidget::readRundownItem(const QString& type, boost::property_tree::wptree& pt, QTreeWidgetItem* parent)
{
    QString deviceName = QString::fromStdWString(pt.get<std::wstring>(L"devicename"));
    QString label = QString::fromStdWString(pt.get<std::wstring>(L"label"));
    QString name = QString::fromStdWString(pt.get<std::wstring>(L"name"));

    AbstractRundownWidget* widget = RundownItemFactory::getInstance().createWidget(LibraryModel(0, label, name, deviceName, type, 0));
    widget->setCompactView(this->compactView);
    widget->getCommand()->readProperties(pt);
    widget->readProperties(pt);

    QTreeWidgetItem* child = new QTreeWidgetItem();
    if (parent == NULL) // Top level item.
        this->treeWidgetRundown->invisibleRootItem()->addChild(child);
    else
    {
        parent->addChild(child);
        widget->setInGroup(true);
    }

    this->treeWidgetRundown->setItemWidget(child, 0, dynamic_cast<QWidget*>(widget));

    if (this->compactView)
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);

    this->treeWidgetRundown->doItemsLayout(); // Refresh.
}

void RundownTreeWidget::writeRundownItem(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item)
{
    AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0));

    QString deviceName = widget->getLibraryModel()->getDeviceName();
    QString label = widget->getLibraryModel()->getLabel();
    QString name = widget->getLibraryModel()->getName();

    writer->writeStartElement("item");

    writer->writeTextElement("type", type);
    writer->writeTextElement("devicename", deviceName);
    writer->writeTextElement("label", label);
    writer->writeTextElement("name", name);

    widget->getCommand()->writeProperties(writer);
    widget->writeProperties(writer);

    writer->writeEndElement();
}

void RundownTreeWidget::checkEmptyRundown()
{
    (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0) ? this->rundownAnimation->start() : this->rundownAnimation->stop();
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
    if (currentItem != NULL && currentItemWidget != NULL)
    {
        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        // Use synchronous event through sendEvent(). The inspector will update the selected
        // rundown item. We want to be absolutely sure that we update the right item, which
        // will not be the case with postEvent() if we trigger up/down arrow key really fast.
        EventManager::getInstance().fireRundownItemSelectedEvent(command, model);
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
    if (currentItem != NULL && currentItemWidget != NULL)
    {
        AbstractCommand* command = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getCommand();
        LibraryModel* model = dynamic_cast<AbstractRundownWidget*>(currentItemWidget)->getLibraryModel();

        // Use synchronous event through sendEvent(). The inspector will update the selected
        // rundown item. We want to be absolutely sure that we update the right item, which
        // will not be the case with postEvent() if we trigger up/down arrow key really fast.
        EventManager::getInstance().fireRundownItemSelectedEvent(command, model);
    }
    else if (currentItem == NULL && previous != NULL && this->treeWidgetRundown->invisibleRootItem()->childCount() == 1) // Last item was removed form the rundown.
    {
        // Use synchronous event through sendEvent(). The inspector will update the selected
        // rundown item. We want to be absolutely sure that we update the right item, which
        // will not be the case with postEvent() if we trigger up/down arrow key really fast.
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

bool RundownTreeWidget::duplicateSelectedItem()
{
    if (!copySelectedItem())
        return false;

    if (!pasteSelectedItem())
        return false;

    return true;
}

bool RundownTreeWidget::copySelectedItem()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return false;

    this->copyItem = this->treeWidgetRundown->currentItem();

    return true;
}

bool RundownTreeWidget::pasteSelectedItem()
{
    if (this->treeWidgetRundown->currentItem() == NULL || this->copyItem == NULL)
        return false;

    if (this->treeWidgetRundown->currentItem()->parent() != NULL && dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->copyItem, 0))->isGroup())
        return false; // We don't support group in groups.

    int row  = this->treeWidgetRundown->currentIndex().row();

    QTreeWidgetItem* parentItem = new QTreeWidgetItem();
    AbstractRundownWidget* parentWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->copyItem, 0))->clone();
    parentWidget->setActive(false);

    if (this->treeWidgetRundown->currentItem()->parent() == NULL) // Top level item.
    {
        parentWidget->setInGroup(false);
        parentWidget->setExpanded(false);

        this->treeWidgetRundown->invisibleRootItem()->insertChild(row + 1, parentItem);
    }
    else
    {
        parentWidget->setInGroup(true);
        parentWidget->setExpanded(true);

        this->treeWidgetRundown->currentItem()->parent()->insertChild(row + 1, parentItem);
    }

    this->treeWidgetRundown->setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

    for (int i = 0; i < this->copyItem->childCount(); i++)
    {
        QTreeWidgetItem* item = this->copyItem->child(i);

        AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->clone();

        QTreeWidgetItem* childItem = new QTreeWidgetItem();
        parentItem->addChild(childItem);
        this->treeWidgetRundown->setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
    }

    if (this->copyItem->isExpanded())
        this->treeWidgetRundown->expandItem(parentItem);

    this->treeWidgetRundown->doItemsLayout(); // Refresh

    return true;
}

bool RundownTreeWidget::moveItemDown()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return false;

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
        return false;

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

bool RundownTreeWidget::executeCommand(Playout::PlayoutType::Type type, ActionSource source)
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return false;

    QWidget* selectedWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0);
    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);

    if (source == GpiPulse && !rundownWidget->getCommand()->getAllowGpi())
        return false; // Gpi pulses cannot trigger this item.

    dynamic_cast<AbstractPlayoutCommand*>(selectedWidget)->executeCommand(type);

    if (rundownWidget->isGroup()) // Group
    {
        for (int i = 0; i < this->treeWidgetRundown->currentItem()->childCount(); i++)
        {
            QWidget* childWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem()->child(i), 0);
            dynamic_cast<AbstractPlayoutCommand*>(childWidget)->executeCommand(type);
        }

        if (dynamic_cast<GroupCommand*>(rundownWidget->getCommand())->getAutoStep() &&
            (type != Playout::PlayoutType::Clear && type != Playout::PlayoutType::ClearVideolayer && type != Playout::PlayoutType::ClearChannel))
            QTimer::singleShot(500, this, SLOT(selectItemBelow()));
    }

    return true;
}

void RundownTreeWidget::selectItemBelow()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return;

    QTreeWidgetItem* itemBelow = NULL;
    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0))->isGroup()) // Group.
        itemBelow = this->treeWidgetRundown->invisibleRootItem()->child(this->treeWidgetRundown->currentIndex().row() + 1);
    else
        itemBelow = this->treeWidgetRundown->itemBelow(this->treeWidgetRundown->currentItem());

    if (itemBelow != NULL)
        this->treeWidgetRundown->setCurrentItem(itemBelow);
}

bool RundownTreeWidget::groupItems()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return false;

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
        return false; // We don't support group in groups.

    QTreeWidgetItem* parentItem = new QTreeWidgetItem();

    RundownGroupWidget* widget = new RundownGroupWidget(LibraryModel(0, "Group", "", "", "GROUP", 0), this);
    widget->setActive(true);
    widget->setExpanded(true);
    widget->setCompactView(this->compactView);

    int row = this->treeWidgetRundown->indexOfTopLevelItem(this->treeWidgetRundown->selectedItems().at(0));
    this->treeWidgetRundown->invisibleRootItem()->insertChild(row, parentItem);
    this->treeWidgetRundown->setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(widget));
    this->treeWidgetRundown->expandItem(parentItem);

    if (this->compactView)
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Define::DEFAULT_ITEM_HEIGHT);

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
        return false;

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
        return false; // We don't have any group to ungroup.

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
    if (this->treeWidgetRundown->currentItem()->parent() == NULL) // Top level item.
        return false;

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
    if (this->treeWidgetRundown->currentItem()->parent() != NULL) // Group item.
        return false;

    if (dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0))->isGroup())
        return false;

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
        this->treeWidgetRundown->doItemsLayout(); // Refresh
        this->treeWidgetRundown->setCurrentItem(newItem);

        delete currentItem;
    }

    return true;
}

bool RundownTreeWidget::removeSelectedItems()
{
    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
       delete item;

    checkEmptyRundown();

    return true;
}

void RundownTreeWidget::parsePage(QKeyEvent* keyEvent)
{
    /*if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        return;

    if (keyEvent->key() == Qt::Key_Plus)
    {
        this->page = QString("%1").arg(this->page.toInt() + 1);
        playSelectedEvent();
    }
    else if (keyEvent->key() == Qt::Key_Minus)
    {
        this->page = QString("%1").arg(this->page.toInt() - 1);
        playSelectedEvent();
    }
    else if (keyEvent->key() >= 48 && keyEvent->key() <= 57)
    {
        if (this->enterPressed)
        {
            this->enterPressed = !this->enterPressed;
            this->page = keyEvent->text();
            playSelectedEvent();
        }
        else
            this->page.append(keyEvent->text());
    }*/
}
