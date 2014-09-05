#include "ImageTreeBaseWidget.h"

#include "EventManager.h"

#include <QtCore/QMimeData>

#include <QtGui/QApplication>
#include <QtGui/QTreeWidgetItem>

ImageTreeBaseWidget::ImageTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent),
      lock(false)
{
    QObject::connect(&EventManager::getInstance(), SIGNAL(repositoryRundown(const RepositoryRundownEvent&)), this, SLOT(repositoryRundown(const RepositoryRundownEvent&)));
}

void ImageTreeBaseWidget::repositoryRundown(const RepositoryRundownEvent& event)
{
    this->lock = event.getRepositoryRundown();
}

void ImageTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void ImageTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (this->lock)
        return;

    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QString data;
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
    {
        data.append(QString("<%1>,,%2,,%3,,%4,,%5,,%6,,%7,,%8;").arg(this->objectName())
                                                                .arg(item->text(0))
                                                                .arg(item->text(1))
                                                                .arg(item->text(2))
                                                                .arg(item->text(3))
                                                                .arg(item->text(4))
                                                                .arg(item->text(5))
                                                                .arg(item->text(6)));
    }
    data.remove(data.length() - 1, 1); // Remove last index of ;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/library-item", data.toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}
