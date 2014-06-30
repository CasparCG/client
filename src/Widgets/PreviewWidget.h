#pragma once

#include "Shared.h"
#include "ui_PreviewWidget.h"

#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtGui/QAction>
#include <QtGui/QImage>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class WIDGETS_EXPORT PreviewWidget : public QWidget, Ui::PreviewWidget
{
    Q_OBJECT

    public:
        explicit PreviewWidget(QWidget* parent = 0);

    private:
        bool viewAlpha;
        bool collapsed;
        QImage image;
        LibraryModel* model;

        QMenu* contextMenuPreviewDropdown;

        QAction* viewAlphaAction;
        QAction* expandCollapseAction;

        void setupMenus();
        void setThumbnail();

        Q_SLOT void toggleExpandCollapse();
        Q_SLOT void viewAlphaChanged(bool);
        Q_SLOT void targetChanged(const TargetChangedEvent&);
        Q_SLOT void libraryItemSelected(const LibraryItemSelectedEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
