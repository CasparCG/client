#include "InspectorRouteChannelWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

InspectorRouteChannelWidget::InspectorRouteChannelWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorRouteChannelWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<RouteChannelCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<RouteChannelCommand*>(event.getCommand());

        this->spinBoxChannel->setValue(this->command->getFromChannel());
    }

    blockAllSignals(false);
}

void InspectorRouteChannelWidget::blockAllSignals(bool block)
{
    this->spinBoxChannel->blockSignals(block);
}

void InspectorRouteChannelWidget::deviceChanged(const DeviceChangedEvent& event)
{
    blockAllSignals(true);

    if (this->command != NULL)
    {
        if (!event.getDeviceName().isEmpty())
        {
            const QStringList& channelFormats = DatabaseManager::getInstance().getDeviceByName(event.getDeviceName()).getChannelFormats().split(",");
            this->spinBoxChannel->setMaximum(channelFormats.count());
        }
    }

    blockAllSignals(false);
}

void InspectorRouteChannelWidget::channelChanged(int fromChannel)
{
    this->command->setFromChannel(fromChannel);
}
