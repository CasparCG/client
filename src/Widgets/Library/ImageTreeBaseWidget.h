#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Events/Rundown/RepositoryRundownEvent.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>

class WIDGETS_EXPORT ImageTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit ImageTreeBaseWidget(QWidget* parent = 0);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool lock;
        QPoint dragStartPosition;

        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
