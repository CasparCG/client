#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemInputWidget.h"

#include "Commands/Atem/AtemInputCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemInputWidget : public QWidget, Ui::InspectorAtemInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemInputWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemInputCommand* command;

        void loadAtemSwitcher();
        void loadAtemInput();
        void blockAllSignals(bool block);

        Q_SLOT void switcherChanged(int);
        Q_SLOT void inputChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
