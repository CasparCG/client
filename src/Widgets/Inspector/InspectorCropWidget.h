#pragma once

#include "../Shared.h"
#include "ui_InspectorCropWidget.h"

#include "Commands/CropCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT InspectorCropWidget : public QWidget, Ui::InspectorCropWidget
{
    Q_OBJECT

    public:
        explicit InspectorCropWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        CropCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();
        void blockAllSignals(bool block);
        void setPositionValues();

        Q_SLOT void sliderLeftChanged(int);
        Q_SLOT void sliderTopChanged(int);
        Q_SLOT void sliderRightChanged(int);
        Q_SLOT void sliderBottomChanged(int);
        Q_SLOT void spinBoxLeftChanged(int);
        Q_SLOT void spinBoxTopChanged(int);
        Q_SLOT void spinBoxRightChanged(int);
        Q_SLOT void spinBoxBottomChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
