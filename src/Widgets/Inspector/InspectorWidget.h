#pragma once

#include "../Shared.h"
#include "ui_InspectorWidget.h"

#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/RepositoryRundownEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorWidget : public QWidget, Ui::InspectorWidget
{
    Q_OBJECT

    public:
        explicit InspectorWidget(QWidget* parent = 0);

    private:
        bool masterVolumeMuted;
        bool disableCommand;

        void setDefaultVisibleWidgets();

        Q_SLOT void masterVolumeClicked();
        Q_SLOT void toggleExpandItem(QTreeWidgetItem*, int);
        Q_SLOT void disableCommandClicked();
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void libraryItemSelected(const LibraryItemSelectedEvent&);
        Q_SLOT void repositoryRundown(const RepositoryRundownEvent&);
};
