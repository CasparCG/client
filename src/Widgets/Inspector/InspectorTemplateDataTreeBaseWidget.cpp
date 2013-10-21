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
    return QStringList("application/dataitem");
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
    if (!mimeData->hasFormat("application/dataitem"))
        return false;

    if (QString::fromUtf8(mimeData->data("application/dataitem")).startsWith("<treeWidgetData>"))
    {
        QStringList dataSplit = QString::fromUtf8(mimeData->data("application/dataitem")).split(":");
        EventManager::getInstance().fireAddTemplateDataEvent(dataSplit.at(1), true);
    }

    return true;
}
