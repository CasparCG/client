#pragma once

#include "../../Shared.h"
#include "ui_InspectorPanasonicPresetWidget.h"
#include "Commands/Panasonic/PanasonicPresetCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QWidget>

class WIDGETS_EXPORT InspectorPanasonicPresetWidget : public QWidget, Ui::InspectorPanasonicPresetWidget
{
    Q_OBJECT
    
    public:
        explicit InspectorPanasonicPresetWidget(QWidget *parent = 0);

    private:
        PanasonicPresetCommand* command;

        void checkEmptyAddress();
        void checkEmptyPreset();
        void blockAllSignals(bool block);

        Q_SLOT void addressChanged(QString);
        Q_SLOT void presetChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
