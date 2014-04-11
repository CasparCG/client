#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemVideoFormatWidget.h"

#include "Commands/Atem/AtemVideoFormatCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemVideoFormatWidget : public QWidget, Ui::InspectorAtemVideoFormatWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemVideoFormatWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemVideoFormatCommand* command;

        void loadAtemFormat();
        void blockAllSignals(bool block);

        Q_SLOT void formatChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
