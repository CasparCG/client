#include "InspectorRouteVideolayerWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/FormatModel.h"

InspectorRouteVideolayerWidget::InspectorRouteVideolayerWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void InspectorRouteVideolayerWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    blockAllSignals(true);

    if (dynamic_cast<RouteVideolayerCommand*>(event.getCommand()))
    {
        this->command = dynamic_cast<RouteVideolayerCommand*>(event.getCommand());

        this->spinBoxChannel->setValue(this->command->getFromChannel());
        this->spinBoxVideolayer->setValue(this->command->getFromVideolayer());
    }

    blockAllSignals(false);
}

void InspectorRouteVideolayerWidget::blockAllSignals(bool block)
{
    this->spinBoxChannel->blockSignals(block);
    this->spinBoxVideolayer->blockSignals(block);
}

void InspectorRouteVideolayerWidget::deviceChanged(const DeviceChangedEvent& event)
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

void InspectorRouteVideolayerWidget::channelChanged(int fromChannel)
{
    this->command->setFromChannel(fromChannel);
}

void InspectorRouteVideolayerWidget::videolayerChanged(int fromVideolayer)
{
    this->command->setFromVideolayer(fromVideolayer);
}
