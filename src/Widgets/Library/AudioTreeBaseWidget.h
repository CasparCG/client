#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Events/Rundown/RepositoryRundownEvent.h"^M

#include <QtGui/QMouseEvent>

#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>

class WIDGETS_EXPORT AudioTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit AudioTreeBaseWidget(QWidget* parent = 0);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool lock;
        QPoint dragStartPosition;

        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
