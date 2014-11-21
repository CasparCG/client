#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Events/Rundown/RepositoryRundownEvent.h"

#include <QtGui/QMouseEvent>

#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>

class WIDGETS_EXPORT ToolTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit ToolTreeBaseWidget(QWidget* parent = 0);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool lock;
        QPoint dragStartPosition;

        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
