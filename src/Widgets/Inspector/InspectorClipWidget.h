#pragma once

#include "../Shared.h"
#include "ui_InspectorClipWidget.h"

#include "Commands/ClipCommand.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorClipWidget : public QWidget, Ui::InspectorClipWidget
{
    Q_OBJECT

    public:
        explicit InspectorClipWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        ClipCommand* command;
        int resolutionWidth;
        int resolutionHeight;

        void loadTween();
        void blockAllSignals(bool block);
        void setPositionValues();

        Q_SLOT void sliderLeftChanged(int);
        Q_SLOT void sliderWidthChanged(int);
        Q_SLOT void sliderTopChanged(int);
        Q_SLOT void sliderHeightChanged(int);
        Q_SLOT void spinBoxLeftChanged(int);
        Q_SLOT void spinBoxWidthChanged(int);
        Q_SLOT void spinBoxTopChanged(int);
        Q_SLOT void spinBoxHeightChanged(int);
        Q_SLOT void transitionDurationChanged(int);
        Q_SLOT void tweenChanged(QString);
        Q_SLOT void deferChanged(int);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
};
