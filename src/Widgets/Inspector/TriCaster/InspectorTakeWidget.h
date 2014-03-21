#pragma once

#include "../../Shared.h"
#include "ui_InspectorTakeWidget.h"

#include "Commands/TriCaster/TakeCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorTakeWidget : public QWidget, Ui::InspectorTakeWidget
{
    Q_OBJECT

    public:
        explicit InspectorTakeWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        TakeCommand* command;

        void checkEmptyStep();
        void loadTriCasterStep();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
