#pragma once

#include "../Shared.h"
#include "ui_InspectorMetadataWidget.h"

#include "Commands/AbstractCommand.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorMetadataWidget : public QWidget, Ui::InspectorMetadataWidget
{
    Q_OBJECT

    public:
        explicit InspectorMetadataWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;

        void blockAllSignals(bool block);

        Q_SLOT void labelChanged(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void libraryItemSelected(const LibraryItemSelectedEvent&);
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
};
