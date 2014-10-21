#pragma once

#include "../Shared.h"
#include "ui_InspectorFillWidget.h"

#include "Commands/FillCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorFillWidget : public QWidget, Ui::InspectorFillWidget
{
    Q_OBJECT

    public:
        explicit InspectorFillWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        FillCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();
        void blockAllSignals(bool block);
        void setScaleAndPositionValues();

        Q_SLOT void sliderPositionXChanged(int);
        Q_SLOT void spinBoxPositionXChanged(int);
        Q_SLOT void sliderPositionYChanged(int);
        Q_SLOT void spinBoxPositionYChanged(int);
        Q_SLOT void sliderScaleXChanged(int);
        Q_SLOT void spinBoxScaleXChanged(int);
        Q_SLOT void sliderScaleYChanged(int);
        Q_SLOT void spinBoxScaleYChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void deferChanged(int);
        Q_SLOT void useMipmapChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
