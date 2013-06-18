#pragma once

#include "../Shared.h"
#include "ui_LibraryWidget.h"

#include "CasparDevice.h"

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
        QMenu* contextMenuImage;
        QMenu* contextMenuPreset;
        QSharedPointer<LibraryModel> model;

        void setupTools();
        void setupUiMenu();

        Q_SLOT void toggleExpandItem(QTreeWidgetItem*, int);
        Q_SLOT bool removeSelectedPresets();
        Q_SLOT void filterLibrary();
        Q_SLOT void contextMenuTriggered(QAction*);
        Q_SLOT void contextMenuImageTriggered(QAction*);
        Q_SLOT void contextMenuPresetTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint& point);
        Q_SLOT void customContextMenuImageRequested(const QPoint& point);
        Q_SLOT void customContextMenuPresetRequested(const QPoint& point);
        Q_SLOT void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
};
