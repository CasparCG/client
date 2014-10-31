#pragma once

#include "../Shared.h"
#include "ui_InspectorRouteChannelWidget.h"

#include "Commands/RouteChannelCommand.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorRouteChannelWidget : public QWidget, Ui::InspectorRouteChannelWidget
{
    Q_OBJECT

    public:
        explicit InspectorRouteChannelWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        RouteChannelCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void channelChanged(int);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
