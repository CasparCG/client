#pragma once

#include "../Shared.h"
#include "ui_InspectorOscOutputWidget.h"
#include "Commands/OscOutputCommand.h"

#include <QWidget>

class WIDGETS_EXPORT InspectorOscOutputWidget : public QWidget, Ui::InspectorOscOutputWidget
{
    Q_OBJECT
    
    public:
        explicit InspectorOscOutputWidget(QWidget *parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        OscOutputCommand* command;

        void loadOscOutput();
        void blockAllSignals(bool block);

        Q_SLOT void outputChanged(QString);
        Q_SLOT void pathChanged(QString);
        Q_SLOT void messageChanged(QString);
        Q_SLOT void typeChanged(QString);
};
