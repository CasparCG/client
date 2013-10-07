#include "RundownTreeBaseWidget.h"

#include "EventManager.h"

#include <QtGui/QApplication>
#include <QtGui/QClipboard>

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

QMimeData* RundownTreeBaseWidget::mimeData(const QList<QTreeWidgetItem*> items) const
{
    QKeyEvent event(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier);
    qApp->sendEvent(qApp, &event);

    QMimeData* mimeData = new QMimeData();
    mimeData->setText(qApp->clipboard()->text());

    return mimeData;
}

bool RundownTreeBaseWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* mimeData, Qt::DropAction action)
{
    //if (parent != NULL)
        QTreeWidget::setCurrentItem(parent);

    if (mimeData->text().contains(":")) // External drop from the library.
    {
        QStringList mimeDataSplit = mimeData->text().split("#");
        foreach(QString data, mimeDataSplit)
        {
            QStringList dataSplit = data.split(":");
            EventManager::getInstance().fireAddRudnownItemEvent(LibraryModel(dataSplit.at(1).toInt(), dataSplit.at(2), dataSplit.at(0),
                                                                             dataSplit.at(3), dataSplit.at(4), dataSplit.at(5).toInt()));
        }
    }
    else
    {
        QKeyEvent w(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier);
        qApp->sendEvent(qApp, &w);

    }

    return true;

    /*
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


    return true;*/
}
