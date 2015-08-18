#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAutoWidget.h"

#include "AtemDevice.h"

#include "Commands/Atem/AtemAutoCommand.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT InspectorAtemAutoWidget : public QWidget, Ui::InspectorAtemAutoWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemAutoWidget(QWidget* parent = 0);

    private:
        quint8 mixerEffects;

        LibraryModel* model;
        AtemAutoCommand* command;

        void checkEmptyStep();
        void checkEmptyMixerStep();
        void loadAtemMixerStep();
        void loadAtemStep();
        void loadAtemAutoTransition();
        void blockAllSignals(bool block);

        Q_SLOT void stepChanged(int);
        Q_SLOT void speedChanged(int);
        Q_SLOT void transitionChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
        Q_SLOT void mixerStepChanged(int);
};
