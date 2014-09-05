#include "PresetTreeBaseWidget.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/PresetChangedEvent.h"

#include <QtCore/QMimeData>

#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QTreeWidgetItem>

PresetTreeBaseWidget::PresetTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent),
      lock(false)
{
    QObject::connect(&EventManager::getInstance(), SIGNAL(repositoryRundown(const RepositoryRundownEvent&)), this, SLOT(repositoryRundown(const RepositoryRundownEvent&)));
}

void PresetTreeBaseWidget::repositoryRundown(const RepositoryRundownEvent& event)
{
    this->lock = event.getRepositoryRundown();
}

void PresetTreeBaseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTreeWidget::mousePressEvent(event);
}

void PresetTreeBaseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (this->lock)
        return;

    if (!(event->buttons() & Qt::LeftButton))
             return;

    if ((event->pos() - dragStartPosition).manhattanLength() < qApp->startDragDistance())
         return;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/library-item", QString("<%1>,,%2,,%3,,%4").arg(this->objectName())
                                                                             .arg(QTreeWidget::selectedItems().at(0)->text(0))
                                                                             .arg(QTreeWidget::selectedItems().at(0)->text(1))
                                                                             .arg(QTreeWidget::selectedItems().at(0)->text(2)).toUtf8());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

void PresetTreeBaseWidget::removeSelectedPresets()
{
    foreach (QTreeWidgetItem* item, QTreeWidget::selectedItems())
        DatabaseManager::getInstance().deletePreset(item->text(1).toInt());

    EventManager::getInstance().firePresetChangedEvent(PresetChangedEvent());
}

void PresetTreeBaseWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
        removeSelectedPresets();
    else
        QTreeWidget::keyPressEvent(event);
}
