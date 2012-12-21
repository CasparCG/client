#pragma once

#include "../Shared.h"
#include "ui_LibraryWidget.h"

#include "Models/LibraryModel.h"

#include <QtCore/QPoint>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class WIDGETS_EXPORT LibraryWidget : public QWidget, Ui::LibraryWidget
{
    Q_OBJECT

    public:
        explicit LibraryWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QMenu* contextMenu;
        QSharedPointer<LibraryModel> model;

        void setupUiMenu();

        Q_SLOT void filterLibrary();
        Q_SLOT void synchronizeLibrary();
        Q_SLOT void contextMenuTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint& point);
        Q_SLOT void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
};
