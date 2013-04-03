#include "RundownWidget.h"
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

#include "DatabaseManager.h"
#include "Events/AddRudnownItemEvent.h"
#include "Events/OpenRundownEvent.h"
#include "Events/SaveRundownEvent.h"
#include "Events/RundownIsEmptyEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/WindowTitleEvent.h"
#include "GpiManager.h"
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
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidgetItem>

RundownWidget::RundownWidget(QWidget* parent)
    : QWidget(parent),
        currentFilename(""), compactView(false), enterPressed(false)
{
    setupUi(this);
    setupUiMenu();

    QToolButton* toolButtonRundownDropdown = new QToolButton(this);
    toolButtonRundownDropdown->setObjectName("toolButtonRundownDropdown");
    this->tabWidgetRundown->setCornerWidget(toolButtonRundownDropdown);
    this->tabWidgetRundown->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));

    this->rundownAnimation = new BorderAnimation(this->treeWidgetRundown);

    // TODO: specific Gpi device.
    connect(GpiManager::getInstance().getGpiDevice().data(), SIGNAL(gpiTriggered(int, GpiDevice*)), this, SLOT(gpiPortTriggered(int, GpiDevice*)));
    connect(this->treeWidgetRundown, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequested(const QPoint &)));

    foreach (const GpiPortModel& port, DatabaseManager::getInstance().getGpiPorts())
        gpiBindingChanged(port.getPort(), port.getAction());

    checkEmptyRundown();

    qApp->installEventFilter(this);
}

void RundownWidget::setupUiMenu()
{
    this->contextMenuMixer = new QMenu(this);
    this->contextMenuMixer->setTitle("Mixer");
    //this->contextMenuMixer->setIcon(QIcon(":/Graphics/Images/Mixer.png"));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BlendModeSmall.png"), "Blend Mode", this, SLOT(addBlendModeCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/BrightnessSmall.png"), "Brightness", this, SLOT(addBrightnessCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/ContrastSmall.png"), "Contrast", this, SLOT(addContrastCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CropSmall.png"), "Crop", this, SLOT(addCropCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/GeometrySmall.png"), "Transformation", this, SLOT(addGeometryCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/GridSmall.png"), "Grid", this, SLOT(addGridCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/KeyerSmall.png"), "Mask", this, SLOT(addKeyerCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/LevelsSmall.png"), "Levels", this, SLOT(addLevelsCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/OpacitySmall.png"), "Opacity", this, SLOT(addOpacityCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/SaturationSmall.png"), "Saturation", this, SLOT(addSaturationCommand()));
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/VolumeSmall.png"), "Volume", this, SLOT(addVolumeCommand()));
    this->contextMenuMixer->addSeparator();
    this->contextMenuMixer->addAction(QIcon(":/Graphics/Images/CommitSmall.png"), "Commit", this, SLOT(addCommitCommand()));

    this->contextMenuLibrary = new QMenu(this);
    this->contextMenuLibrary->setObjectName("contextMenuLibrary");
    this->contextMenuLibrary->setTitle("Library");
    //this->contextMenuLibrary->setIcon(QIcon(":/Graphics/Images/Library.png"));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/AudioSmall.png"), "Audio", this, SLOT(addAudioCommand()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/StillSmall.png"), "Image", this, SLOT(addImageCommand()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/ImageScrollerSmall.png"), "Image Scroller", this, SLOT(addImageScrollerCommand()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/TemplateSmall.png"), "Template", this, SLOT(addTemplateCommand()));
    this->contextMenuLibrary->addAction(QIcon(":/Graphics/Images/MovieSmall.png"), "Video", this, SLOT(addVideoCommand()));

    this->contextMenuNew = new QMenu(this);
    this->contextMenuNew->setTitle("New");
    //this->contextMenuNew->setIcon(QIcon(":/Graphics/Images/New.png"));
    this->contextMenuNew->addMenu(this->contextMenuLibrary);
    this->contextMenuNew->addMenu(this->contextMenuMixer);
    this->contextMenuNew->addSeparator();
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SolidColorSmall.png"), "Solid Color", this, SLOT(addColorProducerCommand()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/GpiOutputSmall.png"), "GPI Output", this, SLOT(addGpiOutputCommand()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/FileRecorderSmall.png"), "File Recorder", this, SLOT(addFileRecorderCommand()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/DeckLinkProducerSmall.png"), "DeckLink Input", this, SLOT(addDeckLinkInputCommand()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SnapshotSmall.png"), "Channel Snapshot", this, SLOT(addPrintCommand()));
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/ClearSmall.png"), "Clear Output", this, SLOT(addClearOutputCommand()));
    this->contextMenuNew->addSeparator();
    this->contextMenuNew->addAction(QIcon(":/Graphics/Images/SeparatorSmall.png"), "Separator", this, SLOT(addSeparatorCommand()));
    //this->contextMenuNew->actions().at(3)->setEnabled(false);

    this->contextMenuColor = new QMenu(this);
    this->contextMenuColor->setTitle("Colorize");
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

bool RundownWidget::eventFilter(QObject* target, QEvent* event)
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
            qApp->postEvent(qApp, new StatusbarEvent("Opening rundown..."));

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

                qDebug() << QString("RundownWidget::eventFilter: Parsing rundown file completed, %1 msec").arg(time.elapsed());

                if (this->treeWidgetRundown->invisibleRootItem()->childCount() > 0)
                    this->treeWidgetRundown->setCurrentItem(this->treeWidgetRundown->invisibleRootItem()->child(0));

                this->treeWidgetRundown->setFocus();
            }

            checkEmptyRundown();

            qDebug() << QString("%1 msec (%2)").arg(time.elapsed()).arg(this->treeWidgetRundown->invisibleRootItem()->childCount());

            this->currentFilename = filename;
            qApp->postEvent(qApp, new WindowTitleEvent(this->currentFilename));
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
            qApp->postEvent(qApp, new StatusbarEvent("Saving rundown..."));

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
            qApp->postEvent(qApp, new WindowTitleEvent(this->currentFilename));
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

        AbstractRundownWidget* widget = NULL;
        if (addRudnownItemEvent->getLibraryModel().getType() == "BLENDMODE")
            widget = new RundownBlendModeWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "BRIGHTNESS")
            widget = new RundownBrightnessWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "CONTRAST")
            widget = new RundownContrastWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "CROP")
            widget = new RundownCropWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "DECKLINKINPUT")
            widget = new RundownDeckLinkInputWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "FILERECORDER")
            widget = new RundownFileRecorderWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "TRANSFORMATION")
            widget = new RundownGeometryWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "GRID")
            widget = new RundownGridWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "GPIOUTPUT")
            widget = new RundownGpiOutputWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "MASK")
            widget = new RundownKeyerWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "LEVELS")
            widget = new RundownLevelsWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "AUDIO")
            widget = new RundownAudioWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "MOVIE")
            widget = new RundownVideoWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "STILL")
            widget = new RundownImageWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "OPACITY")
            widget = new RundownOpacityWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "SATURATION")
            widget = new RundownSaturationWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "TEMPLATE")
            widget = new RundownTemplateWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "VOLUME")
            widget = new RundownVolumeWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "COMMIT")
            widget = new RundownCommitWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "IMAGESCROLLER")
            widget = new RundownImageScrollerWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "SEPARATOR")
            widget = new RundownSeparatorWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "CHANNELSNAPSHOT")
            widget = new RundownPrintWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "CLEAROUTPUT")
            widget = new RundownClearOutputWidget(addRudnownItemEvent->getLibraryModel(), this);
        else if (addRudnownItemEvent->getLibraryModel().getType() == "SOLIDCOLOR")
            widget = new RundownSolidColorWidget(addRudnownItemEvent->getLibraryModel(), this);

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

    return QObject::eventFilter(target, event);
}

void RundownWidget::readRundownGroup(const QString& type, boost::property_tree::wptree& pt)
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

void RundownWidget::writeRundownGroup(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item)
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

void RundownWidget::readRundownItem(const QString& type, boost::property_tree::wptree& pt, QTreeWidgetItem* parent)
{
    QString deviceName = QString::fromStdWString(pt.get<std::wstring>(L"devicename"));
    QString label = QString::fromStdWString(pt.get<std::wstring>(L"label"));
    QString name = QString::fromStdWString(pt.get<std::wstring>(L"name"));

    AbstractRundownWidget* widget = NULL;
    if (type == "BLENDMODE")
        widget = new RundownBlendModeWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "BRIGHTNESS")
        widget = new RundownBrightnessWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "CONTRAST")
        widget = new RundownContrastWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "CROP")
        widget = new RundownCropWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "DECKLINKINPUT")
        widget = new RundownDeckLinkInputWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type ==  "FILERECORDER")
        widget = new RundownFileRecorderWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "TRANSFORMATION")
        widget = new RundownGeometryWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "GRID")
        widget = new RundownGridWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "GPIOUTPUT")
        widget = new RundownGpiOutputWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "MASK")
        widget = new RundownKeyerWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "LEVELS")
        widget = new RundownLevelsWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "AUDIO")
        widget = new RundownAudioWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "STILL")
        widget = new RundownImageWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "MOVIE")
        widget = new RundownVideoWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "OPACITY")
        widget = new RundownOpacityWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "SATURATION")
        widget = new RundownSaturationWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "TEMPLATE")
        widget = new RundownTemplateWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "VOLUME")
        widget = new RundownVolumeWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "COMMIT")
        widget = new RundownCommitWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "IMAGESCROLLER")
        widget = new RundownImageScrollerWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "CHANNELSNAPSHOT")
        widget = new RundownPrintWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "SEPARATOR")
        widget = new RundownSeparatorWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "CLEAROUTPUT")
        widget = new RundownClearOutputWidget(LibraryModel(0, label, name, deviceName, type, 0), this);
    else if (type == "SOLIDCOLOR")
        widget = new RundownSolidColorWidget(LibraryModel(0, label, name, deviceName, type, 0), this);

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

void RundownWidget::writeRundownItem(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item)
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

void RundownWidget::checkEmptyRundown()
{
    (this->treeWidgetRundown->invisibleRootItem()->childCount() == 0) ? this->rundownAnimation->start() : this->rundownAnimation->stop();
}

void RundownWidget::colorizeItems(const QString& color)
{
    if (this->treeWidgetRundown->selectedItems().count() == 0)
        return;

    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
        dynamic_cast<AbstractRundownWidget*>(this->treeWidgetRundown->itemWidget(item, 0))->setColor(color); // Colorize current selected item.
}

void RundownWidget::gpiPortTriggered(int gpiPort, GpiDevice* device)
{
    executeCommand(gpiBindings[gpiPort], GpiPulse);
}

void RundownWidget::gpiBindingChanged(int gpiPort, Playout::PlayoutType::Type binding)
{
    gpiBindings[gpiPort] = binding;
}

void RundownWidget::customContextMenuRequested(const QPoint& point)
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

void RundownWidget::contextMenuNewTriggered(QAction* action)
{
}

void RundownWidget::contextMenuColorTriggered(QAction* action)
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

void RundownWidget::contextMenuRundownTriggered(QAction* action)
{
    if (action->text() == "Group")
        groupItems();
    else if (action->text() == "Ungroup")
        ungroupItems();
}

void RundownWidget::itemClicked(QTreeWidgetItem* current, int i)
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
        qApp->sendEvent(qApp, new RundownItemSelectedEvent(command, model));
    }
}

void RundownWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
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
        qApp->sendEvent(qApp, new RundownItemSelectedEvent(command, model));
    }
    else if (currentItem == NULL && previous != NULL && this->treeWidgetRundown->invisibleRootItem()->childCount() == 1) // Last item was removed form the rundown.
    {
        // Use synchronous event through sendEvent(). The inspector will update the selected
        // rundown item. We want to be absolutely sure that we update the right item, which
        // will not be the case with postEvent() if we trigger up/down arrow key really fast.
        qApp->sendEvent(qApp, new RundownIsEmptyEvent());
    }
}

void RundownWidget::itemDoubleClicked(QTreeWidgetItem* item, int index)
{
    QWidget* selectedWidget = this->treeWidgetRundown->itemWidget(this->treeWidgetRundown->currentItem(), 0);
    AbstractRundownWidget* rundownWidget = dynamic_cast<AbstractRundownWidget*>(selectedWidget);

    if (rundownWidget->isGroup()) // Group.
        rundownWidget->setExpanded(!item->isExpanded());
}

bool RundownWidget::duplicateSelectedItem()
{
    if (!copySelectedItem())
        return false;

    if (!pasteSelectedItem())
        return false;

    return true;
}

bool RundownWidget::copySelectedItem()
{
    if (this->treeWidgetRundown->currentItem() == NULL)
        return false;

    this->copyItem = this->treeWidgetRundown->currentItem();

    return true;
}

bool RundownWidget::pasteSelectedItem()
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

bool RundownWidget::moveItemDown()
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

bool RundownWidget::moveItemUp()
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

bool RundownWidget::executeCommand(Playout::PlayoutType::Type type, ActionSource source)
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

void RundownWidget::selectItemBelow()
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

bool RundownWidget::groupItems()
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

bool RundownWidget::ungroupItems()
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

void RundownWidget::addBlendModeCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Blend Mode", "", "", "BLENDMODE", 0)));
}

void RundownWidget::addBrightnessCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Brightness", "", "", "BRIGHTNESS", 0)));
}

void RundownWidget::addContrastCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Contrast", "", "", "CONTRAST", 0)));
}

void RundownWidget::addCropCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Crop", "", "", "CROP", 0)));
}

void RundownWidget::addImageScrollerCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Image Scroller", "", "", "IMAGESCROLLER", 0)));
}

void RundownWidget::addDeckLinkInputCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "DeckLink Input", "", "", "DECKLINKINPUT", 0)));
}

void RundownWidget::addPrintCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Channel Snapshot", "", "", "CHANNELSNAPSHOT", 0)));
}

void RundownWidget::addClearOutputCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Clear Output", "", "", "CLEAROUTPUT", 0)));
}

void RundownWidget::addGeometryCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Transformation", "", "", "TRANSFORMATION", 0)));
}

void RundownWidget::addGpiOutputCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "GPI Output", "", "", "GPIOUTPUT", 0)));
}

void RundownWidget::addFileRecorderCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "File Recorder", "", "", "FILERECORDER", 0)));
}

void RundownWidget::addSeparatorCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Separator", "", "", "SEPARATOR", 0)));
}

void RundownWidget::addGridCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Grid", "", "", "GRID", 0)));
}

void RundownWidget::addColorProducerCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Solid Color", "", "", "SOLIDCOLOR", 0)));
}

void RundownWidget::addKeyerCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Mask", "", "", "MASK", 0)));
}

void RundownWidget::addLevelsCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Levels", "", "", "LEVELS", 0)));
}

void RundownWidget::addOpacityCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Opacity", "", "", "OPACITY", 0)));
}

void RundownWidget::addSaturationCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Saturation", "", "", "SATURATION", 0)));
}

void RundownWidget::addVolumeCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Volume", "", "", "VOLUME", 0)));
}

void RundownWidget::addCommitCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Commit", "", "", "COMMIT", 0)));
}

void RundownWidget::addAudioCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Audio", "", "", "AUDIO", 0)));
}

void RundownWidget::addImageCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Image", "", "", "STILL", 0)));
}

void RundownWidget::addTemplateCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Template", "", "", "TEMPLATE", 0)));
}

void RundownWidget::addVideoCommand()
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Video", "", "", "MOVIE", 0)));
}

bool RundownWidget::moveItemOutOfGroup()
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

bool RundownWidget::moveItemIntoGroup()
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

bool RundownWidget::removeSelectedItems()
{
    foreach (QTreeWidgetItem* item, this->treeWidgetRundown->selectedItems())
       delete item;

    checkEmptyRundown();

    return true;
}

void RundownWidget::parsePage(QKeyEvent* keyEvent)
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
