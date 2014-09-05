#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Events/Rundown/RepositoryRundownEvent.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>

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
        bool lock;
        QPoint dragStartPosition;

        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
