#include "DataTreeBaseWidget.h"

#include "EventManager.h"

#include <QtCore/QMimeData>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeWidgetItem>
#include <QDrag>
#else
#include <QtGui/QApplication>
#include <QtGui/QTreeWidgetItem>
#endif

DataTreeBaseWidget::DataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent),
      lock(false)
{
    QObject::connect(&EventManager::getInstance(), SIGNAL(repositoryRundown(const RepositoryRundownEvent&)), this, SLOT(repositoryRundown(const RepositoryRundownEvent&)));
}

void DataTreeBaseWidget::repositoryRundown(const RepositoryRundownEvent& event)
{
    this->lock = event.getRepositoryRundown();
}

void DataTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void DataTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (this->lock)
        return;

    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/library-dataitem", QString("<%1>,,%2").arg(this->objectName())
                                                                .arg(QTreeWidget::selectedItems().at(0)->text(2)).toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}
