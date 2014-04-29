#pragma once

#include "../Shared.h"
#include "ui_InspectorOpacityWidget.h"

#include "Commands/OpacityCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorOpacityWidget : public QWidget, Ui::InspectorOpacityWidget
{
    Q_OBJECT

    public:
        explicit InspectorOpacityWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        OpacityCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderOpacityChanged(int);
        Q_SLOT void spinBoxOpacityChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
