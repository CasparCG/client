#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemFadeToBlackWidget.h"

#include "AtemDevice.h"

#include "Commands/Atem/AtemFadeToBlackCommand.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT InspectorAtemFadeToBlackWidget : public QWidget, Ui::InspectorAtemFadeToBlackWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemFadeToBlackWidget(QWidget* parent = 0);

    private:
        quint8 mixerEffects;

        LibraryModel* model;
        AtemFadeToBlackCommand* command;

        void checkEmptyMixerStep();
        void loadAtemMixerStep();
        void blockAllSignals(bool block);

        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
        Q_SLOT void mixerStepChanged(int);
};
