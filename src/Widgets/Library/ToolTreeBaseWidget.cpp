#include "ToolTreeBaseWidget.h"

#include <QtCore/QMimeData>
#include <QtCore/QRegExp>

#include <QtGui/QApplication>
#include <QtGui/QTreeWidgetItem>

ToolTreeBaseWidget::ToolTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

void ToolTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void ToolTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QString data;
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        data.append(QString("<%1>:%2:%3:%4:%5:%6:%7#").arg(this->objectName())
                                                      .arg(item->text(0))
                                                      .arg(item->text(1))
                                                      .arg(item->text(2))
                                                      .arg(item->text(3))
                                                      .arg(item->text(4))
                                                      .arg(item->text(5)));
    }
    data.remove(QRegExp("#$"));

    QMimeData* mimeData = new QMimeData();
    mimeData->setText(data);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}
