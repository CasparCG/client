#pragma once

#include "../Shared.h"
#include "ui_LibraryWidget.h"

#include "CasparDevice.h"

#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/PresetChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Events/Rundown/RepositoryRundownEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QPoint>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QKeyEvent>
#include <QtGui/QWidget>

class WIDGETS_EXPORT LibraryWidget : public QWidget, Ui::LibraryWidget
{
    Q_OBJECT

    public:
        explicit LibraryWidget(QWidget* parent = 0);

    private:
        QMenu* contextMenu;
        QMenu* contextMenuImage;
        QMenu* contextMenuPreset;
        QMenu* contextMenuData;
        QSharedPointer<LibraryModel> model;

        void setupTools();
        void setupUiMenu();

        Q_SLOT void loadLibrary();
        Q_SLOT void toggleExpandItem(QTreeWidgetItem*, int);
        Q_SLOT void filterLibrary();
        Q_SLOT void contextMenuTriggered(QAction*);
        Q_SLOT void contextMenuImageTriggered(QAction*);
        Q_SLOT void contextMenuPresetTriggered(QAction*);
        Q_SLOT void contextMenuDataTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint&);
        Q_SLOT void customContextMenuImageRequested(const QPoint&);
        Q_SLOT void customContextMenuPresetRequested(const QPoint&);
        Q_SLOT void customContextMenuDataRequested(const QPoint&);
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
        Q_SLOT void mediaChanged(const MediaChangedEvent&);
        Q_SLOT void templateChanged(const TemplateChangedEvent&);
        Q_SLOT void dataChanged(const DataChangedEvent&);
        Q_SLOT void presetChanged(const PresetChangedEvent&);
        Q_SLOT void importPreset(const ImportPresetEvent&);
        Q_SLOT void exportPreset(const ExportPresetEvent&);
        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
