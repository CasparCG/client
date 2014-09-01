#include "RundownTreeBaseWidget.h"
#include "RundownItemFactory.h"
#include "RundownGroupWidget.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Commands/VideoCommand.h"
#include "Events/Rundown/AllowRemoteTriggeringEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/RemoveItemFromAutoPlayQueueEvent.h"
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

bool RundownTreeBaseWidget::copyItemProperties() const
{
    copySelectedItems();
}

bool RundownTreeBaseWidget::pasteItemProperties()
{
    qDebug() << qApp->clipboard()->text();
    std::wstringstream wstringstream;
    wstringstream << qApp->clipboard()->text().toStdWString();

    boost::property_tree::wptree pt;
    boost::property_tree::xml_parser::read_xml(wstringstream, pt);

    BOOST_FOREACH(boost::property_tree::wptree::value_type& parentValue, pt.get_child(L"items"))
    {
        QString type = QString::fromStdWString(parentValue.second.get(L"type", L""));

        foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
        {
            AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
            if (widget->getLibraryModel()->getType() == type)
                widget->getCommand()->readProperties(parentValue.second);
        }
    }

    return true;
}

bool RundownTreeBaseWidget::pasteSelectedItems(bool repositoryRundown)
{
    std::wstringstream wstringstream;
    wstringstream << qApp->clipboard()->text().toStdWString();

    int offset = 1; // Drop offset.
    boost::property_tree::wptree pt;
    boost::property_tree::xml_parser::read_xml(wstringstream, pt);

    bool allowRemoteTriggering = pt.get(L"items.allowremotetriggering", false);
    EventManager::getInstance().fireAllowRemoteTriggeringEvent(AllowRemoteTriggeringEvent(allowRemoteTriggering));

    EventManager::getInstance().fireAllowRemoteTriggeringMenuEvent(AllowRemoteTriggeringMenuEvent(!repositoryRundown));

    BOOST_FOREACH(boost::property_tree::wptree::value_type& parentValue, pt.get_child(L"items"))
    {
        if (parentValue.first != L"item")
            continue;

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

void RundownTreeBaseWidget::addRepositoryItem(const QString& storyId, const QString& data)
{
    int row = -1;

    for (int i = QTreeWidget::topLevelItemCount() - 1; i >= 0; i--)
    {
        QTreeWidgetItem* item = QTreeWidget::topLevelItem(i);
        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
        if (widget->getCommand()->getStoryId() == storyId)
            row = QTreeWidget::indexFromItem(item).row();
    }

    int offset = 1;
    std::wstringstream wstringstream;
    wstringstream << data.toStdWString();

    boost::property_tree::wptree pt;
    boost::property_tree::xml_parser::read_xml(wstringstream, pt);

    bool allowRemoteTriggering = pt.get(L"items.allowremotetriggering", false);
    EventManager::getInstance().fireAllowRemoteTriggeringEvent(AllowRemoteTriggeringEvent(allowRemoteTriggering));
    EventManager::getInstance().fireAllowRemoteTriggeringMenuEvent(AllowRemoteTriggeringMenuEvent(false));

    BOOST_FOREACH(boost::property_tree::wptree::value_type& parentValue, pt.get_child(L"items"))
    {
        if (parentValue.first != L"item")
            continue;

        AbstractRundownWidget* parentWidget = readProperties(parentValue.second);

        QTreeWidgetItem* parentItem = new QTreeWidgetItem();
        if (QTreeWidget::currentItem() == NULL || QTreeWidget::currentItem()->parent() == NULL) // Top level item.
        {
            parentWidget->setInGroup(false);
            parentWidget->setExpanded(false);

            // If we don't have a selected row then we add the item to the bottom of the
            // rundown. This can be the case when we drag and drop a preset to the rundown.
            if (row == -1)
                QTreeWidget::invisibleRootItem()->insertChild(0, parentItem);
            else
                QTreeWidget::invisibleRootItem()->insertChild(row + offset++, parentItem);
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
            //bool expanded = parentValue.second.get(L"expanded", false);
            //parentItem->setExpanded(expanded);

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
}


bool RundownTreeBaseWidget::duplicateSelectedItems()
{
    // Save the latest value stored in the clipboard.
    QString latest = qApp->clipboard()->text();

    if (!copySelectedItems())
        return true;

    if (!pasteSelectedItems())
        return true;

    // Set previous stored clipboard value.
    qApp->clipboard()->setText(latest);

    return true;
}

void RundownTreeBaseWidget::checkEmptyRundown()
{
    if (DatabaseManager::getInstance().getConfigurationByName("Theme").getValue() == Appearance::CURVE_THEME)
        QTreeWidget::setStyleSheet((QTreeWidget::invisibleRootItem()->childCount() == 0) ? "#treeWidgetRundown { border-width: 1; border-color: firebrick; }" : "#treeWidgetRundown { border-width: 1; }");
    else
        QTreeWidget::setStyleSheet((QTreeWidget::invisibleRootItem()->childCount() == 0) ? "#treeWidgetRundown { border-width: 1; border-color: firebrick; }" : "#treeWidgetRundown { border-width: 0; border-top-width: 1; }");
}

void RundownTreeBaseWidget::checRepositoryUpdates()
{
    if (DatabaseManager::getInstance().getConfigurationByName("Theme").getValue() == Appearance::CURVE_THEME)
        QTreeWidget::setStyleSheet((QTreeWidget::invisibleRootItem()->childCount() > 0) ? "#treeWidgetRundown { border-width: 1; border-color: darkorange; }" : "#treeWidgetRundown { border-width: 1; }");
    else
        QTreeWidget::setStyleSheet((QTreeWidget::invisibleRootItem()->childCount() > 0) ? "#treeWidgetRundown { border-width: 1; border-color: darkorange; }" : "#treeWidgetRundown { border-width: 0; border-top-width: 1; }");
}

void RundownTreeBaseWidget::removeRepositoryItem(const QString& storyId)
{
    for (int i = QTreeWidget::topLevelItemCount() - 1; i >= 0; i--)
    {
        QTreeWidgetItem* item = QTreeWidget::topLevelItem(i);
        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
        if (widget->getCommand()->getStoryId() == storyId)
        {
            if (hasItemBelow())
                selectItemBelow();
            else
                selectItemAbove();

            AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
            if (widget->isGroup())
            {
                for (int i = item->childCount() - 1; i >= 0; i--)
                {
                    QWidget* childWidget = QTreeWidget::itemWidget(item->child(i), 0);

                    // Remove our items from the AutoPlay queue if they exists.
                    EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item->child(i)));

                    delete childWidget;
                    delete item->child(i);
                }
            }

            // Remove our items from the AutoPlay queue if they exists.
            EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item));

            delete widget;
            delete item;
        }
    }

    checkEmptyRundown();
    checRepositoryUpdates();
}

void RundownTreeBaseWidget::removeSelectedItems()
{
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
        if (widget->isGroup())
        {
            for (int i = item->childCount() - 1; i >= 0; i--)
            {
                QWidget* childWidget = QTreeWidget::itemWidget(item->child(i), 0);

                // Remove our items from the auto play queue if they exists.
                EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item->child(i)));

                delete childWidget;
                delete item->child(i);
            }
        }

        // Remove our items from the AutoPlay queue if they exists.
        EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item));

        delete widget;
        delete item;
    }

    checkEmptyRundown();
}

void RundownTreeBaseWidget::removeAllItems()
{
    for (int i = QTreeWidget::invisibleRootItem()->childCount() - 1; i >= 0; i--)
    {
        QTreeWidgetItem* item = QTreeWidget::invisibleRootItem()->child(i);
        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0));
        if (widget->isGroup())
        {
            for (int i = item->childCount() - 1; i >= 0; i--)
            {
                QWidget* childWidget = QTreeWidget::itemWidget(item->child(i), 0);

                // Remove our items from the auto play queue if they exists.
                EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item->child(i)));

                delete childWidget;
                delete item->child(i);
            }
        }

        // Remove our items from the auto play queue if they exists.
        EventManager::getInstance().fireRemoveItemFromAutoPlayQueueEvent(RemoveItemFromAutoPlayQueueEvent(item));

        delete widget;
        delete item;
    }

    checkEmptyRundown();
}

void RundownTreeBaseWidget::groupItems()
{
    if (QTreeWidget::currentItem() == NULL)
        return;

    bool isGroup = false;
    bool isTopItem = false;
    bool isGroupItem = false;
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        QWidget* widget = QTreeWidget::itemWidget(item, 0);

        if (item->parent() != NULL) // Group item.
            isGroupItem = true;
        else if (dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Group
            isGroup = true;
        else if (item->parent() == NULL && !dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Top level item.
            isTopItem = true;
    }

    if (isGroup || isGroupItem)
        return; // We don't support group in groups.

    QTreeWidgetItem* parentItem = new QTreeWidgetItem();

    RundownGroupWidget* widget = new RundownGroupWidget(LibraryModel(0, "Group", "", "", "GROUP", 0, ""), this);
    widget->setActive(true);
    widget->setExpanded(true);
    widget->setCompactView(getCompactView());

    int row = QTreeWidget::indexOfTopLevelItem(QTreeWidget::selectedItems().at(0));
    QTreeWidget::invisibleRootItem()->insertChild(row, parentItem);
    QTreeWidget::setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(widget));
    QTreeWidget::expandItem(parentItem);

    if (getCompactView())
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::COMPACT_ITEM_HEIGHT);
    else
        dynamic_cast<QWidget*>(widget)->setFixedHeight(Rundown::DEFAULT_ITEM_HEIGHT);

    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        QTreeWidgetItem* childItem = new QTreeWidgetItem();
        parentItem->addChild(childItem);

        AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0))->clone();
        childWidget->setInGroup(true);
        childWidget->setActive(false);

        QTreeWidget::setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
    }

    removeSelectedItems();

    QTreeWidget::doItemsLayout(); // Refresh
    QTreeWidget::setCurrentItem(parentItem);
}

void RundownTreeBaseWidget::ungroupItems()
{
    if (QTreeWidget::currentItem() == NULL)
        return;

    bool isGroup = false;
    bool isTopItem = false;
    bool isGroupItem = false;
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        QWidget* widget = QTreeWidget::itemWidget(item, 0);

        if (item->parent() != NULL) // Group item.
            isGroupItem = true;
        else if (dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Group
            isGroup = true;
        else if (item->parent() == NULL && !dynamic_cast<AbstractRundownWidget*>(widget)->isGroup()) // Top level item.
            isTopItem = true;
    }

    if (isTopItem || (isGroup && isGroupItem) || (isTopItem && isGroupItem))
        return; // We don't have any group to ungroup.

    QTreeWidgetItem* rootItem = QTreeWidget::invisibleRootItem();

    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0))->isGroup()) // Group.
    {
        QTreeWidgetItem* currentItem = QTreeWidget::currentItem();
        QTreeWidgetItem* currentItemAbove = QTreeWidget::itemAbove(QTreeWidget::currentItem());

        int row = QTreeWidget::indexOfTopLevelItem(QTreeWidget::selectedItems().at(0));

        QTreeWidgetItem* newItem = NULL;
        for (int i = 0; i < QTreeWidget::currentItem()->childCount(); i++)
        {
            QTreeWidgetItem* item = QTreeWidget::currentItem()->child(i);

            newItem = new QTreeWidgetItem();
            rootItem->insertChild(row + 1, newItem);

            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0))->clone();
            newWidget->setInGroup(false);
            newWidget->setActive(false);

            QTreeWidget::setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));

            row++;
        }

        QTreeWidget::setCurrentItem(currentItemAbove);

        delete currentItem;
    }
    else // Group item.
    {
        QTreeWidgetItem* parentItem = QTreeWidget::currentItem()->parent();

        int parentRow = QTreeWidget::indexOfTopLevelItem(QTreeWidget::currentItem()->parent());

        QTreeWidgetItem* newItem = NULL;
        foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
        {
            newItem = new QTreeWidgetItem();
            rootItem->insertChild(parentRow + 1, newItem);

            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0))->clone();
            newWidget->setInGroup(false);
            newWidget->setActive(false);

            QTreeWidget::setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));

            delete item;

            parentRow++;
        }

        QTreeWidget::setCurrentItem(newItem);

        if (parentItem->childCount() == 0)
            delete parentItem;
    }

    QTreeWidget::doItemsLayout(); // Refresh
}

void RundownTreeBaseWidget::moveItemUp()
{
    if (QTreeWidget::currentItem() == NULL)
        return;

    int row  = QTreeWidget::currentIndex().row();
    QTreeWidgetItem* currentItem = QTreeWidget::currentItem();
    QTreeWidgetItem* parentItem = QTreeWidget::currentItem()->parent();

    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->isGroup())
    {
        int rowCount = 0;
        if (currentItem != NULL && row > rowCount)
        {
            AbstractRundownWidget* parentWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();
            parentWidget->setInGroup(true);
            parentWidget->setExpanded(true);

            QTreeWidgetItem* parentItem = new QTreeWidgetItem();
            QTreeWidget::invisibleRootItem()->insertChild(row - 1, parentItem);
            QTreeWidget::setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

            if (QTreeWidget::currentItem()->isExpanded())
                QTreeWidget::expandItem(parentItem);

            for (int i = 0; i < QTreeWidget::currentItem()->childCount(); i++)
            {
                QTreeWidgetItem* item = QTreeWidget::currentItem()->child(i);

                AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0))->clone();
                childWidget->setInGroup(true);

                QTreeWidgetItem* childItem = new QTreeWidgetItem();
                parentItem->addChild(childItem);
                QTreeWidget::setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
            }

            delete currentItem;

            QTreeWidget::setCurrentItem(parentItem);
            QTreeWidget::doItemsLayout(); // Refresh
        }
    }
    else
    {
        int rowCount = 0;
        if (currentItem != NULL && row > rowCount)
        {
            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();

            if (parentItem == NULL) // Top level item.
            {
                QTreeWidget::invisibleRootItem()->takeChild(row);
                QTreeWidget::invisibleRootItem()->insertChild(row - 1, currentItem);
            }
            else // Group item.
            {
                newWidget->setInGroup(true);

                QTreeWidget::currentItem()->parent()->takeChild(row);
                QTreeWidget::currentItem()->parent()->insertChild(row - 1, currentItem);
            }

            QTreeWidget::setItemWidget(currentItem, 0, dynamic_cast<QWidget*>(newWidget));
            QTreeWidget::setCurrentItem(currentItem);
            QTreeWidget::doItemsLayout(); // Refresh
        }
    }
}

void RundownTreeBaseWidget::moveItemDown()
{
    if (QTreeWidget::currentItem() == NULL)
        return;

    int row  = QTreeWidget::currentIndex().row();
    QTreeWidgetItem* currentItem = QTreeWidget::currentItem();
    QTreeWidgetItem* parentItem = QTreeWidget::currentItem()->parent();

    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->isGroup())
    {
        int rowCount = 0;
        if (parentItem == NULL) // Top level item.
            rowCount = QTreeWidget::invisibleRootItem()->childCount() - 1;

        if (currentItem != NULL && row < rowCount)
        {
            AbstractRundownWidget* parentWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();
            parentWidget->setInGroup(true);
            parentWidget->setExpanded(true);

            QTreeWidgetItem* parentItem = new QTreeWidgetItem();
            QTreeWidget::invisibleRootItem()->insertChild(row + 2, parentItem);
            QTreeWidget::setItemWidget(parentItem, 0, dynamic_cast<QWidget*>(parentWidget));

            if (QTreeWidget::currentItem()->isExpanded())
                QTreeWidget::expandItem(parentItem);

            for (int i = 0; i < QTreeWidget::currentItem()->childCount(); i++)
            {
                QTreeWidgetItem* item = QTreeWidget::currentItem()->child(i);

                AbstractRundownWidget* childWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(item, 0))->clone();
                childWidget->setInGroup(true);

                QTreeWidgetItem* childItem = new QTreeWidgetItem();
                parentItem->addChild(childItem);
                QTreeWidget::setItemWidget(childItem, 0, dynamic_cast<QWidget*>(childWidget));
            }

            delete currentItem;

            QTreeWidget::setCurrentItem(parentItem);
            QTreeWidget::doItemsLayout(); // Refresh
        }
    }
    else
    {
        int rowCount = 0;
        if (parentItem == NULL) // Top level item.
            rowCount = QTreeWidget::invisibleRootItem()->childCount() - 1;
        else
            rowCount = QTreeWidget::currentItem()->parent()->childCount() - 1;

        if (currentItem != NULL && row < rowCount)
        {
            AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();

            if (parentItem == NULL) // Top level item.
            {
                QTreeWidget::invisibleRootItem()->takeChild(row);
                QTreeWidget::invisibleRootItem()->insertChild(row + 1, currentItem);
            }
            else // Group item.
            {
                newWidget->setInGroup(true);

                QTreeWidget::currentItem()->parent()->takeChild(row);
                QTreeWidget::currentItem()->parent()->insertChild(row + 1, currentItem);
            }

            QTreeWidget::setItemWidget(currentItem, 0, dynamic_cast<QWidget*>(newWidget));
            QTreeWidget::setCurrentItem(currentItem);
            QTreeWidget::doItemsLayout(); // Refresh
        }
    }
}

void RundownTreeBaseWidget::moveItemOutOfGroup()
{
    if (QTreeWidget::currentItem() == NULL || QTreeWidget::currentItem()->parent() == NULL) // Top level item.
        return;

    QTreeWidgetItem* newItem = new QTreeWidgetItem();
    QTreeWidgetItem* currentItem = QTreeWidget::currentItem();
    QTreeWidgetItem* parentItem = QTreeWidget::currentItem()->parent(); // Group.
    QTreeWidgetItem* currentItemAbove = QTreeWidget::itemAbove(QTreeWidget::currentItem());
    QTreeWidgetItem* parentItemAbove = QTreeWidget::itemAbove(QTreeWidget::currentItem()->parent());

    int currentRow  = QTreeWidget::currentIndex().row();
    int parentRow  = QTreeWidget::indexOfTopLevelItem(QTreeWidget::currentItem()->parent());

    AbstractRundownWidget* newWidget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();
    newWidget->setInGroup(false);

    QTreeWidget::currentItem()->parent()->takeChild(currentRow);
    QTreeWidget::invisibleRootItem()->insertChild(parentRow + 1, newItem);
    QTreeWidget::setItemWidget(newItem, 0, dynamic_cast<QWidget*>(newWidget));
    QTreeWidget::setCurrentItem(newItem);
    QTreeWidget::doItemsLayout(); // Refresh

    delete currentItem;

    if (parentItem->childCount() == 0)
    {
        QTreeWidget::setCurrentItem(parentItemAbove);
        delete parentItem;
    }
}

void RundownTreeBaseWidget::moveItemIntoGroup()
{
    if (QTreeWidget::currentItem() == NULL || QTreeWidget::currentItem()->parent() != NULL) // Group item.
        return;

    if (dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(QTreeWidget::currentItem(), 0))->isGroup())
        return;

    QTreeWidgetItem* currentItemAbove = QTreeWidget::invisibleRootItem()->child(QTreeWidget::currentIndex().row() - 1);
    if (currentItemAbove != NULL && dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItemAbove, 0))->isGroup()) // Group.
    {
        QTreeWidgetItem* newItem = new QTreeWidgetItem();
        QTreeWidgetItem* currentItem = QTreeWidget::currentItem();

        int currentRow  = QTreeWidget::currentIndex().row();

        AbstractRundownWidget* widget = dynamic_cast<AbstractRundownWidget*>(QTreeWidget::itemWidget(currentItem, 0))->clone();
        widget->setInGroup(true);

        currentItemAbove->addChild(newItem);

        QTreeWidget::invisibleRootItem()->takeChild(currentRow);
        QTreeWidget::setItemWidget(newItem, 0, dynamic_cast<QWidget*>(widget));
        QTreeWidget::doItemsLayout(); // Ref resh
        QTreeWidget::setCurrentItem(newItem);

        delete currentItem;
    }
}

void RundownTreeBaseWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
        removeSelectedItems();
    else if (event->key() == Qt::Key_D && event->modifiers() == Qt::ControlModifier)
        duplicateSelectedItems();
    else if (event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
        copySelectedItems();
    else if (event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier)
        pasteSelectedItems();
    else if (event->key() == Qt::Key_G && event->modifiers() == Qt::ControlModifier)
        groupItems();
    else if (event->key() == Qt::Key_U && event->modifiers() == Qt::ControlModifier)
        ungroupItems();
    else if (event->key() == Qt::Key_X && event->modifiers() == Qt::ControlModifier)
    {
        copySelectedItems();
        removeSelectedItems();
    }
    else if (event->key() == Qt::Key_Up && (event->modifiers() == Qt::ControlModifier || (event->modifiers() & Qt::ControlModifier && event->modifiers() & Qt::KeypadModifier)))
        moveItemUp();
    else if (event->key() == Qt::Key_Down && (event->modifiers() == Qt::ControlModifier || (event->modifiers() & Qt::ControlModifier && event->modifiers() & Qt::KeypadModifier)))
        moveItemDown();
    else if (event->key() == Qt::Key_Left && (event->modifiers() == Qt::ControlModifier || (event->modifiers() & Qt::ControlModifier && event->modifiers() & Qt::KeypadModifier)))
        moveItemOutOfGroup();
    else if (event->key() == Qt::Key_Right && (event->modifiers() == Qt::ControlModifier || (event->modifiers() & Qt::ControlModifier && event->modifiers() & Qt::KeypadModifier)))
        moveItemIntoGroup();
    else
        QTreeWidget::keyPressEvent(event);
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
                QStringList dataSplit = data.split(",,");
                EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(dataSplit.at(2).toInt(), dataSplit.at(3), dataSplit.at(1),
                                                                                 dataSplit.at(4), dataSplit.at(5), dataSplit.at(6).toInt(),
                                                                                 dataSplit.at(7)));
            }
        }
        else if (dndData.startsWith("<treeWidgetPreset>")) // External drop from the preset library.
        {
            QTreeWidget::setCurrentItem(parent);

            QStringList dataSplit = dndData.split(",,");
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
