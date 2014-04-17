#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAudioGainWidget.h"

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

        void loadAtemSource();
        void blockAllSignals(bool block);

        Q_SLOT void sourceChanged(int);
        Q_SLOT void sliderGainChanged(int);
        Q_SLOT void doubleSpinBoxGainChanged(double);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
