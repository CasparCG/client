#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAudioGainWidget.h"

#include "AtemDevice.h"

#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Commands/Atem/AtemAudioGainCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemAudioGainWidget : public QWidget, Ui::InspectorAtemAudioGainWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemAudioGainWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemAudioGainCommand* command;
        QMap<quint16, QAtemConnection::InputInfo> inputs;

        void blockAllSignals(bool block);
        void loadAtemAudioInput(QMap<quint16, QAtemConnection::InputInfo> inputs);

        Q_SLOT void inputChanged(int);
        Q_SLOT void sliderGainChanged(int);
        Q_SLOT void doubleSpinBoxGainChanged(double);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
};
