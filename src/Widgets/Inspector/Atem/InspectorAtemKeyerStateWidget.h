#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemKeyerStateWidget.h"

#include "AtemDevice.h"

#include "Commands/Atem/AtemKeyerStateCommand.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT InspectorAtemKeyerStateWidget : public QWidget, Ui::InspectorAtemKeyerStateWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemKeyerStateWidget(QWidget* parent = 0);

    private:
        quint8 mixerEffects;

        LibraryModel* model;
        AtemKeyerStateCommand* command;

        void checkEmptyMixerStep();
        void loadAtemKeyer();
        void loadAtemMixerStep();
        void blockAllSignals(bool block);

        Q_SLOT void keyerChanged(int);
        Q_SLOT void stateChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
        Q_SLOT void mixerStepChanged(int);
};
