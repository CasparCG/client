#pragma once

#include "../Shared.h"
#include "ui_InspectorBrightnessWidget.h"

#include "Commands/BrightnessCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorBrightnessWidget : public QWidget, Ui::InspectorBrightnessWidget
{
    Q_OBJECT

    public:
        explicit InspectorBrightnessWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        BrightnessCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderBrightnessChanged(int);
        Q_SLOT void spinBoxBrightnessChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
