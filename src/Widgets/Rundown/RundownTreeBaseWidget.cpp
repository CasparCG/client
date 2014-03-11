#include "RundownTreeBaseWidget.h"
#include "RundownItemFactory.h"
#include "RundownGroupWidget.h"

#include "EventManager.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/RemoteRundownTriggeringEvent.h"
#include "Models/LibraryModel.h"

#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QPainter>

#include <QDebug>

RundownTreeBaseWidget::RundownTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent), compactView(false)
{
}

bool RundownTreeBaseWidget::getCompactView() const
{
    return this->compactView;
}

void RundownTreeBaseWidget::setCompactView(bool compactView)
{
    this->compactView = compactView;
}

void RundownTreeBaseWidget::writeProperties(QTreeWidgetItem* item, QXmlStreamWriter* writer) const
{
    AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
    if (widget->getLibraryModel()->getType() == "GROUP")
    {
        QString label = widget->getLibraryModel()->getLabel();

        writer->writeStartElement("item");
        writer->writeTextElement("type", widget->getLibraryModel()->getType());
        writer->writeTextElement("label", label);
        writer->writeTextElement("expanded", (item->isExpanded() == true ? "true" : "false"));
        widget->getCommand()->writeProperties(writer);
        widget->writeProperties(writer);

        writer->writeStartElement("items");
        for (int i = 0; i < item->childCount(); i++)
            writeProperties(item->child(i), writer);

        writer->writeEndElement();
        writer->writeEndElement();
    }
    else
    {
        QString deviceName = widget->getLibraryModel()->getDeviceName();
        QString label = widget->getLibraryModel()->getLabel();
        QString name = widget->getLibraryModel()->getName();

        writer->writeStartElement("item");
        writer->writeTextElement("type", widget->getLibraryModel()->getType());
        writer->writeTextElement("devicename", deviceName);
        writer->writeTextElement("label", label);
        writer->writeTextElement("name", name);
        widget->getCommand()->writeProperties(writer);
        widget->writeProperties(writer);
        writer->writeEndElement();
    }
}

AbstractRundownWidget* RundownTreeBaseWidget::readProperties(boost::property_tree::wptree& pt)
{
    QString type = QString::fromStdWString(pt.get(L"type", L""));

    AbstractRundownWidget* widget = NULL;
    if (type == "GROUP")
    {
        QString label = QString::fromStdWString(pt.get(L"label", L""));

        widget = new RundownGroupWidget(LibraryModel(0, label, "", "", type, 0, ""), this);
        widget->setExpanded(true);
        widget->setCompactView(this->compactView);
        widget->getCommand()->readProperties(pt);
        widget->readProperties(pt);
    }
    else
    {
        QString deviceName = QString::fromStdWString(pt.get(L"devicename", L""));
        QString label = QString::fromStdWString(pt.get(L"label", L""));
        QString name = QString::fromStdWString(pt.get(L"name", L""));

        widget = RundownItemFactory::getInstance().createWidget(LibraryModel(0, label, name, deviceName, type, 0, ""));
        widget->setCompactView(this->compactView);
        widget->getCommand()->readProperties(pt);
        widget->readProperties(pt);
    }

    if (this->compactView)
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);

    return widget;
}

bool RundownTreeBaseWidget::copySelectedItems() const
{
    QString data;
    QXmlStreamWriter* writer = new QXmlStreamWriter(&data);

    writer->writeStartDocument();
    writer->writeStartElement("items");
    for (int i = 0; i < QTreeWidget::selectedItems().count(); i++)
        writeProperties(QTreeWidget::selectedItems().at(i), writer);

    writer->writeEndElement();
    writer->writeEndDocument();

    qApp->clipboard()->setText(data);

    delete writer;

    return true;
}

bool RundownTreeBaseWidget::pasteSelectedItems()
{
    std::wstringstream wstringstream;
    wstringstream << qApp->clipboard()->text().toStdWString();

    int offset = 1;
    boost::property_tree::wptree pt;
    boost::property_tree::xml_parser::read_xml(wstringstream, pt);

    BOOST_FOREACH(boost::property_tree::wptree::value_type& parentValue, pt.get_child(L"items"))
    {
        if (parentValue.first != L"item")
            continue;

        bool allowRemoteTriggering = parentValue.second.get(L"allowremotetriggering", false);
        EventManager::getInstance().fireRemoteRundownTriggeringEvent(RemoteRundownTriggeringEvent(allowRemoteTriggering));
        EventManager::getInstance().fireAllowRemoteTriggeringMenuEvent(AllowRemoteTriggeringMenuEvent(allowRemoteTriggering));

        AbstractRundownWidget* parentWidget = readProperties(parentValue.second);

        int row  = QTreeWidget::currentIndex().row();

        QTreeWidgetItem* parentItem = new QTreeWidgetItem();
        if (QTreeWidget::currentItem() == NULL || QTreeWidget::currentItem()->parent() == NULL) // Top level item.
        {
            parentWidget->setInGroup(false);
            parentWidget->setExpanded(false);

            // If we don't have a selected row then we add the item to the bottom of the
            // rundown. This can be the case when we drag and drop a preset to the rundown.
            if (row != -1)
                QTreeWidget::invisibleRootItem()->insertChild(row + offset++, parentItem);
            else
                QTreeWidget::invisibleRootItem()->addChild(parentItem);
        }
        else
        {
            if (parentWidget->isGroup())
                continue; // We don't support group in groups.

            parentWidget->setInGroup(true);

            QTreeWidget::currentItem()->parent()->insertChild(row + offset++, parentItem);
        }

        QTreeWidget::setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

        if (parentWidget->isGroup())
        {
            bool expanded = parentValue.second.get(L"expanded", false);
            parentItem->setExpanded(expanded);

            BOOST_FOREACH(boost::property_tree::wptree::value_type& childValue, parentValue.second.get_child(L"items"))
            {
                AbstractRundownWidget* childWidget = readProperties(childValue.second);
                childWidget->setInGroup(true);

                QTreeWidgetItem* childItem = new QTreeWidgetItem();
                parentItem->addChild(childItem);

                QTreeWidget::setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
            }
        }

        QTreeWidget::doItemsLayout(); // Refresh
    }

    return true;
}

bool RundownTreeBaseWidget::duplicateSelectedItems()
{
    if (!copySelectedItems())
        return true;

    if (!pasteSelectedItems())
        return true;

    return true;
}

void RundownTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void RundownTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    if (!copySelectedItems())
        return;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/rundown-item", qApp->clipboard()->text().toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

Qt::DropActions RundownTreeBaseWidget::supportedDropActions() const
{
    return Qt::CopyAction;
}

QStringList RundownTreeBaseWidget::mimeTypes() const
{
    QStringList list;
    list.append("application/library-item");
    list.append("application/rundown-item");

    return list;
}

void RundownTreeBaseWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

bool RundownTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    if (!mimeData->hasFormat("application/library-item") && !mimeData->hasFormat("application/rundown-item"))
        return false;

    if (mimeData->hasFormat("application/library-item"))
    {
        QString dndData = QString::fromUtf8(mimeData->data("application/library-item"));
        if (dndData.startsWith("<treeWidgetVideo>") ||
            dndData.startsWith("<treeWidgetTool>") ||
            dndData.startsWith("<treeWidgetTemplate>") ||
            dndData.startsWith("<treeWidgetImage>") ||
            dndData.startsWith("<treeWidgetAudio>")) // External drop from the library.
        {
            QTreeWidget::setCurrentItem(parent);

            QStringList dndDataSplit = dndData.split(";");
            foreach(QString data, dndDataSplit)
            {
                QStringList dataSplit = data.split(",");
                EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(dataSplit.at(2).toInt(), dataSplit.at(3), dataSplit.at(1),
                                                                                 dataSplit.at(4), dataSplit.at(5), dataSplit.at(6).toInt(),
                                                                                 dataSplit.at(7)));
            }
        }
        else if (dndData.startsWith("<treeWidgetPreset>")) // External drop from the preset library.
        {
            QTreeWidget::setCurrentItem(parent);

            QStringList dataSplit = dndData.split(",");
            EventManager::getInstance().fireAddPresetItemEvent(AddPresetItemEvent(dataSplit.at(3)));
        }
    }
    else if (mimeData->hasFormat("application/rundown-item"))
    {
        QString dndData = QString::fromUtf8(mimeData->data("application/rundown-item"));
        if (dndData.contains("<items>")) // Internal drop
        {
            QList<QTreeWidgetItem*> items = QTreeWidget::selectedItems();

            QTreeWidget::setCurrentItem(parent);

            if (!pasteSelectedItems())
                return false;

            selectItemBelow();

            foreach (QTreeWidgetItem* item, items)
               delete item;
        }
    }

    return true;
}

bool RundownTreeBaseWidget::hasItemBelow() const
{
    if (QTreeWidget::currentItem() == NULL)
        return false;

    QTreeWidgetItem* itemBelow = NULL;
    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0))->isGroup()) // Group.
        itemBelow = QTreeWidget::invisibleRootItem()->child(QTreeWidget::currentIndex().row() + 1);
    else
        itemBelow = QTreeWidget::itemBelow(QTreeWidget::currentItem());

    return (itemBelow == NULL) ? false : true;
}

void RundownTreeBaseWidget::selectItemAbove()
{
    if (QTreeWidget::currentItem() == NULL)
    {
        QTreeWidget::setCurrentItem(QTreeWidget::invisibleRootItem()->child(0));
        return;
    }

    QTreeWidgetItem* itemAbove = NULL;
    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0))->isGroup()) // Group.
        itemAbove = QTreeWidget::invisibleRootItem()->child(QTreeWidget::currentIndex().row() - 1);
    else
        itemAbove = QTreeWidget::itemAbove(QTreeWidget::currentItem());

    if (itemAbove != NULL)
    {
        // Inactivate the current item.
        QWidget* itemWidget = QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0);
        if (itemWidget != NULL)
            dynamic_cast<AbstractRundownWidget*>(itemWidget)->setActive(false);

        QTreeWidget::setCurrentItem(itemAbove);

        // Activate item above.
        QWidget* itemAboveWidget = QTreeWidget::itemWidget(itemAbove, 0);
        if (itemAboveWidget != NULL)
            dynamic_cast<AbstractRundownWidget*>(itemAboveWidget)->setActive(true);
    }
}

void RundownTreeBaseWidget::selectItemBelow()
{
    if (QTreeWidget::currentItem() == NULL)
    {
        QTreeWidget::setCurrentItem(QTreeWidget::invisibleRootItem()->child(QTreeWidget::invisibleRootItem()->childCount() - 1));
        return;
    }

    QTreeWidgetItem* itemBelow = NULL;
    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0))->isGroup()) // Group.
        itemBelow = QTreeWidget::invisibleRootItem()->child(QTreeWidget::currentIndex().row() + 1);
    else
        itemBelow = QTreeWidget::itemBelow(QTreeWidget::currentItem());

    if (itemBelow != NULL)
    {
        // Inactivate the current item.
        QWidget* itemWidget = QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0);
        if (itemWidget != NULL)
            dynamic_cast<AbstractRundownWidget*>(itemWidget)->setActive(false);

        QTreeWidget::setCurrentItem(itemBelow);

        // Activate item below.
        QWidget* itemBelowWidget = QTreeWidget::itemWidget(itemBelow, 0);
        if (itemBelowWidget != NULL)
            dynamic_cast<AbstractRundownWidget*>(itemBelowWidget)->setActive(true);
    }
}
