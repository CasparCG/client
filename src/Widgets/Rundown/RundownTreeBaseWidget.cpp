#include "RundownTreeBaseWidget.h"

#include "EventManager.h"

RundownTreeBaseWidget::RundownTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

Qt::DropActions RundownTreeBaseWidget::supportedDropActions () const
{
    return Qt::CopyAction;
}

QStringList RundownTreeBaseWidget::mimeTypes () const
{
    return QStringList("text/plain");
}

void RundownTreeBaseWidget::dragEnterEvent(QDragEnterEvent* event)
 {
    event->acceptProposedAction();
 }

void RundownTreeBaseWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    QTreeWidget::dragLeaveEvent(event);
}

bool RundownTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    if (!mimeData->hasText())
        return false;

    if (!mimeData->text().contains(":"))
        return false;

    if (parent != NULL)
        QTreeWidget::setCurrentItem(parent);

    QStringList mimeDataSplit = mimeData->text().split("#");
    foreach(QString data, mimeDataSplit)
    {
        QStringList dataSplit = data.split(":");
        EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(dataSplit.at(1).toInt(), dataSplit.at(2), dataSplit.at(0),
                                                                         dataSplit.at(3), dataSplit.at(4), dataSplit.at(5).toInt()));
    }

    return true;
}
