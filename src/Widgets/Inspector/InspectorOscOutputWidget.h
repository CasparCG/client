#pragma once

#include "../Shared.h"
#include "ui_InspectorOscOutputWidget.h"
#include "Commands/OscOutputCommand.h"
#include "Events/OscOutputChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"

#include <QWidget>

class WIDGETS_EXPORT InspectorOscOutputWidget : public QWidget, Ui::InspectorOscOutputWidget
{
    Q_OBJECT
    
    public:
        explicit InspectorOscOutputWidget(QWidget *parent = 0);

    private:
        OscOutputCommand* command;

        void checkEmptyOutput();
        void checkEmptyPath();
        void loadOscOutput();
        void blockAllSignals(bool block);

        Q_SLOT void outputChanged(QString);
        Q_SLOT void pathChanged(QString);
        Q_SLOT void messageChanged(QString);
        Q_SLOT void typeChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void oscOutputChanged(const OscOutputChangedEvent&);
        Q_SLOT void useBundleChanged(int);
};
