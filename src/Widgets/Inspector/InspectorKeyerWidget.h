#pragma once

#include "../Shared.h"
#include "ui_InspectorKeyerWidget.h"

#include "Commands/KeyerCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorKeyerWidget : public QWidget, Ui::InspectorKeyerWidget
{
    Q_OBJECT

    public:
        explicit InspectorKeyerWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        KeyerCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void deferChanged(int);
        Q_SLOT void resetDefer(QString);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
