#pragma once

#include "../../Shared.h"
#include "ui_InspectorInputWidget.h"

#include "Commands/TriCaster/InputCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorInputWidget : public QWidget, Ui::InspectorInputWidget
{
    Q_OBJECT

    public:
        explicit InspectorInputWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        InputCommand* command;

        void loadTriCasterSwitcher();
        void loadTriCasterInput();
        void blockAllSignals(bool block);

        Q_SLOT void switcherChanged(int);
        Q_SLOT void inputChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
