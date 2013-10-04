#include "DataTreeBaseWidget.h"

#include <QtCore/QRegExp>

DataTreeBaseWidget::DataTreeBaseWidget(QWidget* parent)
    : QTreeWidget(parent)
{
}

QMimeData* DataTreeBaseWidget::mimeData(const QList<QTreeWidgetItem*> items) const
{
    QMimeData* mimeData = new QMimeData();
    mimeData->setText(items.at(0)->text(0));

    return mimeData;
}
