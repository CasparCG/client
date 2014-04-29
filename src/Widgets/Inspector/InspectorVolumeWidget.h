#pragma once

#include "../Shared.h"
#include "ui_InspectorVolumeWidget.h"

#include "Commands/VolumeCommand.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorVolumeWidget : public QWidget, Ui::InspectorVolumeWidget
{
    Q_OBJECT

    public:
        explicit InspectorVolumeWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        VolumeCommand* command;

        void loadTween();
        void blockAllSignals(bool block);

        Q_SLOT void sliderVolumeChanged(int);
        Q_SLOT void spinBoxVolumeChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
