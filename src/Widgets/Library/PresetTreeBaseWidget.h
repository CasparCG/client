#pragma once

#include "../Shared.h"

#include "Global.h"

#if QT_VERSION >= 0x050000
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#else
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#endif

class WIDGETS_EXPORT PresetTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit PresetTreeBaseWidget(QWidget* parent = 0);

        void removeSelectedPresets();

    protected:
        void keyPressEvent(QKeyEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        QPoint dragStartPosition;
};
