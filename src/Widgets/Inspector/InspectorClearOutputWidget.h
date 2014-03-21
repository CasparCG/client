#pragma once

#include "../Shared.h"
#include "ui_InspectorClearOutputWidget.h"

#include "Commands/ClearOutputCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorClearOutputWidget : public QWidget, Ui::InspectorClearOutputWidget
{
    Q_OBJECT

    public:
        explicit InspectorClearOutputWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        ClearOutputCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void clearChannelChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void resetClearChannel(QString);
        Q_SLOT void resetTriggerOnNext(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
