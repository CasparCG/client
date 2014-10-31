#pragma once

#include "../Shared.h"
#include "ui_InspectorRouteVideolayerWidget.h"

#include "Commands/RouteVideolayerCommand.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT InspectorRouteVideolayerWidget : public QWidget, Ui::InspectorRouteVideolayerWidget
{
    Q_OBJECT

    public:
        explicit InspectorRouteVideolayerWidget(QWidget* parent = 0);

    private:
        LibraryModel* model;
        RouteVideolayerCommand* command;

        void blockAllSignals(bool block);

        Q_SLOT void channelChanged(int);
        Q_SLOT void videolayerChanged(int);
        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
};
