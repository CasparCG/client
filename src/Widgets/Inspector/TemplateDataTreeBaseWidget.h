#pragma once

#include "../Shared.h"

#include "Global.h"

#include <QtCore/QMimeData>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QDragEnterEvent>
#else
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QDragEnterEvent>
#endif

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
