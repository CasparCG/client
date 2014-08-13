#include "TemplateDataTreeBaseWidget.h"

#include "EventManager.h"
#include "Events/Inspector/AddTemplateDataEvent.h"

#include <QtGui/QApplication>
#include <QtGui/QTreeWidgetItem>

TemplateDataTreeBaseWidget::TemplateDataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

void TemplateDataTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void TemplateDataTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QString data;
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        data.append(QString("<%1>,,%2,,%3;").arg(this->objectName())
                                            .arg(item->text(0))
                                            .arg(item->text(1)));
    }
    data.remove(data.length() - 1, 1); // Remove last index of ;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/inspector-templatedataitem", data.toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

Qt::DropActions TemplateDataTreeBaseWidget::supportedDropActions() const
{
    return Qt::CopyAction;
}

QStringList TemplateDataTreeBaseWidget::mimeTypes() const
{
    QStringList list;
    list.append("application/library-dataitem");
    list.append("application/inspector-templatedataitem");

    return list;
}

void TemplateDataTreeBaseWidget::dragEnterEvent(QDragEnterEvent* event)
 {
    event->acceptProposedAction();
 }

void TemplateDataTreeBaseWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    QTreeWidget::dragLeaveEvent(event);
}

bool TemplateDataTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    if (!mimeData->hasFormat("application/library-dataitem") && !mimeData->hasFormat("application/inspector-templatedataitem"))
        return false;

    if (mimeData->hasFormat("application/library-dataitem"))
    {
        QString dndData = QString::fromUtf8(mimeData->data("application/library-dataitem"));
        if (dndData.startsWith("<treeWidgetData>"))
        {
            QStringList dataSplit = dndData.split(",,");
            EventManager::getInstance().fireAddTemplateDataEvent(AddTemplateDataEvent(dataSplit.at(1), true));
        }
    }
    else if (mimeData->hasFormat("application/inspector-templatedataitem"))
    {
        QString dndData = QString::fromUtf8(mimeData->data("application/inspector-templatedataitem"));
        if (dndData.startsWith("<treeWidgetTemplateData>"))
        {
            QStringList dataSplit = dndData.split(",,");

            QTreeWidgetItem* treeItem = new QTreeWidgetItem();
            treeItem->setText(0, dataSplit.at(1));
            treeItem->setText(1, dataSplit.at(2));

            if (parent == NULL) // Drop on the QTreeWidget.
                QTreeWidget::invisibleRootItem()->addChild(treeItem);
            else
                QTreeWidget::invisibleRootItem()->insertChild(QTreeWidget::indexFromItem(parent).row() + 1, treeItem);

            delete QTreeWidget::currentItem();

            QTreeWidget::setCurrentItem(treeItem);
        }
    }

    return true;
}
