#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemCutWidget.h"

#include "Commands/Atem/AtemCutCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemCutWidget : public QWidget, Ui::InspectorAtemCutWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemCutWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemCutCommand* command;

        void checkEmptyStep();
        void loadAtemStep();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
