#pragma once

#include "../Shared.h"

#include "Global.h"

#if QT_VERSION >= 0x050000
#include <QMouseEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#else
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#endif

class WIDGETS_EXPORT VideoTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit VideoTreeBaseWidget(QWidget* parent = 0);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        QPoint dragStartPosition;
};
