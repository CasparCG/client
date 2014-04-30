#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"

#include "Global.h"

#include "OscSubscription.h"
#include "Events/AddPresetItemEvent.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QModelIndexList>
#include <QtCore/QMimeData>
#include <QtCore/QRect>
#include <QtCore/QXmlStreamWriter>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPixmap>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

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

        bool pasteSelectedItems();
        bool duplicateSelectedItems();
        bool copySelectedItems() const;
        bool hasItemBelow() const;

        void moveItemUp();
        void moveItemDown();
        void moveItemIntoGroup();
        void moveItemOutOfGroup();
        void groupItems();
        void ungroupItems();
        void removeSelectedItems();
        void removeAllItems();
        void selectItemAbove();
        void selectItemBelow();
        void checkEmptyRundown();

        virtual bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);

    protected:
        void keyPressEvent(QKeyEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool compactView;

        QPoint dragStartPosition;
};
