#pragma once

#include "../../Shared.h"
#include "ui_InspectorSpyderPresetWidget.h"
#include "Commands/Spyder/SpyderPresetCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QWidget>

class WIDGETS_EXPORT InspectorSpyderPresetWidget : public QWidget, Ui::InspectorSpyderPresetWidget
{
    Q_OBJECT
    
    public:
        explicit InspectorSpyderPresetWidget(QWidget *parent = 0);

    private:
        SpyderPresetCommand* command;

        void checkEmptyAddress();
        void blockAllSignals(bool block);

        Q_SLOT void addressChanged(QString);
        Q_SLOT void presetChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
