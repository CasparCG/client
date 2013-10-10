#include "InspectorTemplateDataTreeBaseWidget.h"

#include "EventManager.h"

#include <QtCore/QRegExp>

InspectorTemplateDataTreeBaseWidget::InspectorTemplateDataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

Qt::DropActions InspectorTemplateDataTreeBaseWidget::supportedDropActions () const
{
    return Qt::CopyAction;
}

QStringList InspectorTemplateDataTreeBaseWidget::mimeTypes () const
{
    return QStringList("text/plain");
}

void InspectorTemplateDataTreeBaseWidget::dragEnterEvent(QDragEnterEvent* event)
 {
    event->acceptProposedAction();
 }

void InspectorTemplateDataTreeBaseWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    QTreeWidget::dragLeaveEvent(event);
}

bool InspectorTemplateDataTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    if (!mimeData->hasText())
        return false;

    if (mimeData->text().startsWith("<treeWidgetData>"))
    {
        QStringList dataSplit = mimeData->text().split(":");
        EventManager::getInstance().fireAddTemplateDataEvent(dataSplit.at(1), true);
    }

    return true;
}
