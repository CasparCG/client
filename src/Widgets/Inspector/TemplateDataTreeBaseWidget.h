#pragma once

#include "../Shared.h"

#include "Global.h"

#include <QtCore/QMimeData>

#include <QtGui/QDragEnterEvent>

#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

class WIDGETS_EXPORT TemplateDataTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit TemplateDataTreeBaseWidget(QWidget* parent = 0);

        QStringList mimeTypes () const;
        Qt::DropActions supportedDropActions () const;
        void dragEnterEvent(QDragEnterEvent* event);
        void dragLeaveEvent(QDragLeaveEvent* event);
        virtual bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        QPoint dragStartPosition;
};
