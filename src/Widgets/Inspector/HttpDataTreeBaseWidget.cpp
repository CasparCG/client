#include "HttpDataTreeBaseWidget.h"

#include "EventManager.h"

#include <QtGui/QApplication>
#include <QtGui/QTreeWidgetItem>

HttpDataTreeBaseWidget::HttpDataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

void HttpDataTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void HttpDataTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
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
    mimeData->setData("application/inspector-httpdataitem", data.toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

Qt::DropActions HttpDataTreeBaseWidget::supportedDropActions() const
{
    return Qt::CopyAction;
}

QStringList HttpDataTreeBaseWidget::mimeTypes() const
{
    QStringList list;
    list.append("application/inspector-httpdataitem");

    return list;
}

void HttpDataTreeBaseWidget::dragEnterEvent(QDragEnterEvent* event)
 {
    event->acceptProposedAction();
 }

void HttpDataTreeBaseWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    QTreeWidget::dragLeaveEvent(event);
}

bool HttpDataTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    if (!mimeData->hasFormat("application/inspector-httpdataitem"))
        return false;

    QString dndData = QString::fromUtf8(mimeData->data("application/inspector-httpdataitem"));
    if (dndData.startsWith("<treeWidgetHttpData>"))
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

    return true;
}
