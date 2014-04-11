#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAutoWidget.h"

#include "Commands/Atem/AtemAutoCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemAutoWidget : public QWidget, Ui::InspectorAtemAutoWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemAutoWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemAutoCommand* command;

        void checkEmptyStep();
        void loadAtemStep();
        void loadAtemAutoTransition();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
        Q_SLOT void speedChanged(int);
        Q_SLOT void transitionChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
