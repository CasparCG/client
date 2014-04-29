#pragma once

#include "../Shared.h"
#include "ui_InspectorSaturationWidget.h"

#include "Commands/SaturationCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorSaturationWidget : public QWidget, Ui::InspectorSaturationWidget
{
    Q_OBJECT

    public:
        explicit InspectorSaturationWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        SaturationCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderSaturationChanged(int);
        Q_SLOT void spinBoxSaturationChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
