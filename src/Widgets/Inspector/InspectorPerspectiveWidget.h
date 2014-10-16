#pragma once

#include "../Shared.h"
#include "ui_InspectorPerspectiveWidget.h"

#include "Commands/PerspectiveCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorPerspectiveWidget : public QWidget, Ui::InspectorPerspectiveWidget
{
    Q_OBJECT

    public:
        explicit InspectorPerspectiveWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        PerspectiveCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();
        void blockAllSignals(bool block);
        void setPositionValues();

        Q_SLOT void sliderUpperLeftXChanged(int);
        Q_SLOT void sliderUpperLeftYChanged(int);
        Q_SLOT void sliderUpperRightXChanged(int);
        Q_SLOT void sliderUpperRightYChanged(int);
        Q_SLOT void sliderLowerLeftXChanged(int);
        Q_SLOT void sliderLowerLeftYChanged(int);
        Q_SLOT void sliderLowerRightXChanged(int);
        Q_SLOT void sliderLowerRightYChanged(int);
        Q_SLOT void spinBoxUpperLeftXChanged(int);
        Q_SLOT void spinBoxUpperLeftYChanged(int);
        Q_SLOT void spinBoxUpperRightXChanged(int);
        Q_SLOT void spinBoxUpperRightYChanged(int);
        Q_SLOT void spinBoxLowerLeftXChanged(int);
        Q_SLOT void spinBoxLowerLeftYChanged(int);
        Q_SLOT void spinBoxLowerRightXChanged(int);
        Q_SLOT void spinBoxLowerRightYChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void triggerOnNextChanged(int);
        Q_SLOT void deferChanged(int);
        Q_SLOT void useMipmapChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
