#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/TemplateChangedEvent.h"
#include "Models/DeviceModel.h"

#include "CasparDevice.h"

#include <stdexcept>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(DeviceManager, deviceManager)

DeviceManager::DeviceManager()
{
}

DeviceManager& DeviceManager::getInstance()
{
    return *deviceManager();
}

void DeviceManager::initialize()
{
    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();
    foreach (const DeviceModel& model, models)
    {
        QSharedPointer<CasparDevice> device(new CasparDevice());
        QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
        device->connect(model.getAddress(), model.getPort());

        this->deviceModels.insert(model.getName(), model);
        this->connections.insert(model.getName(), device);

        emit deviceAdded(*device);
    }
}

void DeviceManager::uninitialize()
{
    foreach (const QString& key, this->connections.keys())
    {
        QSharedPointer<CasparDevice>& connection = this->connections[key];
        connection->disconnect();
    }
}

void DeviceManager::synchronize()
{
    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();

    // Disconnect old devices.
    foreach (const QString& key, this->connections.keys())
    {
        QSharedPointer<CasparDevice>& connection = this->connections[key];

        bool foundDevice = false;
        foreach (DeviceModel model, models)
        {
            if (model.getAddress() == connection->getAddress())
            {
                foundDevice = true;
                break;
            }
        }

        if (!foundDevice)
        {
            connection->disconnect();
            this->connections.remove(key);
            this->deviceModels.remove(key);

            emit deviceRemoved();
        }
    }

    // Connect new devices.
    foreach (DeviceModel model, models)
    {
        if (!this->connections.contains(model.getName()))
        {
            QSharedPointer<CasparDevice> device(new CasparDevice());
            QObject::connect(device.data(), SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
            device->connect(model.getAddress(), model.getPort());

            this->deviceModels.insert(model.getName(), model);
            this->connections.insert(model.getName(), device);

            emit deviceAdded(*device);
        }
    }
}

QList<DeviceModel> DeviceManager::getDeviceModels() const
{
    QList<DeviceModel> models;
    foreach (const DeviceModel& model, this->deviceModels)
        models.push_back(model);

    return models;
}

const DeviceModel DeviceManager::getDeviceModelByAddress(const QString& address) const
{
    foreach (const DeviceModel& model, this->deviceModels)
    {
        if (model.getAddress() == address)
            return model;
    }

    throw std::runtime_error("No DeviceModel found for specified address");
}

const int DeviceManager::getConnectionCount() const
{
    return this->connections.count();
}

const QSharedPointer<CasparDevice> DeviceManager::getConnectionByName(const QString& name) const
{
    return this->connections.value(name);
}

void DeviceManager::deviceConnectionStateChanged(CasparDevice& device)
{
    qApp->postEvent(qApp, new ConnectionStateChangedEvent(getDeviceModelByAddress(device.getAddress()).getName(), device.isConnected()));
}
