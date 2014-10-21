#pragma once

#include "../Shared.h"
#include "ui_InspectorAnchorWidget.h"

#include "Commands/AnchorCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorAnchorWidget : public QWidget, Ui::InspectorAnchorWidget
{
    Q_OBJECT

    public:
        explicit InspectorAnchorWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        AnchorCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();
        void blockAllSignals(bool block);
        void setPositionValues();

        Q_SLOT void sliderPositionXChanged(int);
        Q_SLOT void spinBoxPositionXChanged(int);
        Q_SLOT void sliderPositionYChanged(int);
        Q_SLOT void spinBoxPositionYChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
