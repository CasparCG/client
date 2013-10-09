#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QRect>
#include <QtCore/QModelIndexList>
#include <QtGui/QStyleOptionViewItemV4>

#include <QtCore/QMimeData>
#include <QtCore/QXmlStreamWriter>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QPixmap>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

typedef QPair<QRect, QModelIndex> QItemViewPaintPair;
typedef QList<QItemViewPaintPair> QItemViewPaintPairs;

class WIDGETS_EXPORT RundownTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit RundownTreeBaseWidget(QWidget* parent = 0);

        bool getCompactView() const;
        void setCompactView(bool compactView);

        QStringList mimeTypes() const;
        Qt::DropActions supportedDropActions() const;
        void dragEnterEvent(QDragEnterEvent* event);

        AbstractRundownWidget* readProperties(boost::property_tree::wptree& pt);
        void writeProperties(QTreeWidgetItem* item, QXmlStreamWriter* writer) const;

        virtual bool pasteSelectedItems();
        virtual bool duplicateSelectedItems();
        virtual bool copySelectedItems() const;
        virtual bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool compactView;
        QPoint dragStartPosition;
};
