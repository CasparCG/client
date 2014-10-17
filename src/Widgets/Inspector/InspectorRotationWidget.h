#pragma once

#include "../Shared.h"
#include "ui_InspectorRotationWidget.h"

#include "Commands/RotationCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorRotationWidget : public QWidget, Ui::InspectorRotationWidget
{
    Q_OBJECT

    public:
        explicit InspectorRotationWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        RotationCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void sliderRotationChanged(int);
        Q_SLOT void doubleSpinBoxRotationChanged(double);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
