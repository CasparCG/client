#pragma once

#include "../../Shared.h"
#include "ui_InspectorSonyPresetWidget.h"
#include "Commands/Sony/SonyPresetCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QWidget>

class WIDGETS_EXPORT InspectorSonyPresetWidget : public QWidget, Ui::InspectorSonyPresetWidget
{
    Q_OBJECT
    
    public:
        explicit InspectorSonyPresetWidget(QWidget *parent = 0);

    private:
        SonyPresetCommand* command;

        void checkEmptyAddress();
        void checkEmptyPreset();
        void blockAllSignals(bool block);

        Q_SLOT void addressChanged(QString);
        Q_SLOT void presetChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
