#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAudioInputStateWidget.h"

#include "AtemDevice.h"

#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Commands/Atem/AtemAudioInputStateCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemAudioInputStateWidget : public QWidget, Ui::InspectorAtemAudioInputStateWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemAudioInputStateWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemAudioInputStateCommand* command;
        QMap<quint16, QAtemConnection::InputInfo> inputs;

        void loadAtemInputState();
        void blockAllSignals(bool block);
        void loadAtemAudioInput(QMap<quint16, QAtemConnection::InputInfo> inputs);

        Q_SLOT void inputChanged(int);
        Q_SLOT void stateChanged(int);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void atemDeviceChanged(const AtemDeviceChangedEvent&);
};
