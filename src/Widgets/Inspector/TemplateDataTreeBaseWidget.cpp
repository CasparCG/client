#include "TemplateDataTreeBaseWidget.h"

#include "EventManager.h"

#include <QtCore/QRegExp>

TemplateDataTreeBaseWidget::TemplateDataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

Qt::DropActions TemplateDataTreeBaseWidget::supportedDropActions () const
{
    return Qt::CopyAction;
}

QStringList TemplateDataTreeBaseWidget::mimeTypes () const
{
    return QStringList("text/plain");
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
    if (!mimeData->hasText())
        return false;

    EventManager::getInstance().fireAddTemplateDataEvent(mimeData->text(), true);

    return true;
}
