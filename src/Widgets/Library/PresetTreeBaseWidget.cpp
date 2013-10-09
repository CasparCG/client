#include "PresetTreeBaseWidget.h"

#include <QtCore/QMimeData>
#include <QtCore/QRegExp>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QTreeWidgetItem>

PresetTreeBaseWidget::PresetTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

void PresetTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void PresetTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QMimeData* mimeData = new QMimeData();
    mimeData->setText(QString("%1-%2-%3").arg(QTreeWidget::selectedItems().at(0)->text(0))
                                         .arg(QTreeWidget::selectedItems().at(0)->text(1))
                                         .arg(QTreeWidget::selectedItems().at(0)->text(2)));

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}
