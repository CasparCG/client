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
    return QStringList("application/library-dataitem");
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
    if (!mimeData->hasFormat("application/library-dataitem"))
        return false;

    QString dndData = QString::fromUtf8(mimeData->data("application/library-dataitem"));
    if (dndData.startsWith("<treeWidgetData>"))
    {
        QStringList dataSplit = dndData.split(":");
        EventManager::getInstance().fireAddTemplateDataEvent(dataSplit.at(1), true);
    }

    return true;
}
