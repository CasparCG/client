#include "ToolTreeBaseWidget.h"

#include <QtCore/QRegExp>

ToolTreeBaseWidget::ToolTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

QMimeData* ToolTreeBaseWidget::mimeData(const QList<QTreeWidgetItem*> items) const
{
    QString data;
    foreach (QTreeWidgetItem* item, items)
    {
        data.append(QString("%1:%2:%3:%4:%5:%6#").arg(item->text(1)).arg(item->text(2)).arg(item->text(0))
                                                 .arg(item->text(3)).arg(item->text(4)).arg(item->text(5)));
    }
    data.remove(QRegExp("#$"));

    QMimeData* mimeData = new QMimeData();
    mimeData->setText(data);

    return mimeData;
}
