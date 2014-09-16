#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Events/Rundown/RepositoryRundownEvent.h"^M 

#if QT_VERSION >= 0x050000
#include <QMouseEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#else
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#endif

class WIDGETS_EXPORT TemplateTreeBaseWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit TemplateTreeBaseWidget(QWidget* parent = 0);

    protected:
        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);

    private:
        bool lock;
        QPoint dragStartPosition;

        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
