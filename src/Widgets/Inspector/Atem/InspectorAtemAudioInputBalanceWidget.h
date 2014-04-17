#pragma once

#include "../../Shared.h"
#include "ui_InspectorAtemAudioInputBalanceWidget.h"

#include "Commands/Atem/AtemAudioInputBalanceCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAtemAudioInputBalanceWidget : public QWidget, Ui::InspectorAtemAudioInputBalanceWidget
{
    Q_OBJECT

    public:
        explicit InspectorAtemAudioInputBalanceWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AtemAudioInputBalanceCommand* command;

        void loadAtemInput();
        void blockAllSignals(bool block);

        Q_SLOT void inputChanged(int);
        Q_SLOT void sliderBalanceChanged(int);
        Q_SLOT void doubleSpinBoxBalanceChanged(double);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
