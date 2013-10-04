#pragma once

#include "../Shared.h"

#include "Global.h"

#include <QtCore/QMimeData>

#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

class WIDGETS_EXPORT VideoTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit VideoTreeBaseWidget(QWidget* parent = 0);

    protected:
        virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
};
