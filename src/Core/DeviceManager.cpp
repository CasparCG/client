#include "DeviceManager.h"
#include "DatabaseManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Models/DeviceModel.h"

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
        QSharedPointer<CasparDevice> device(new CasparDevice(model.getAddress(), model.getPort()));

        this->deviceModels.insert(model.getName(), model);
        this->devices.insert(model.getName(), device);

        emit deviceAdded(*device);

        device->connectDevice();
    }
}

void DeviceManager::uninitialize()
{
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<CasparDevice>& device = this->devices[key];
        device->disconnectDevice();
    }
}

void DeviceManager::refresh()
{
    QList<DeviceModel> models = DatabaseManager::getInstance().getDevice();

    // Disconnect old devices.
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<CasparDevice>& device = this->devices[key];

        bool foundDevice = false;
        foreach (DeviceModel model, models)
        {
            if (model.getAddress() == device->getAddress())
            {
                foundDevice = true;
                break;
            }
        }

        if (!foundDevice)
        {
            device->disconnectDevice();

            this->devices.remove(key);
            this->deviceModels.remove(key);

            emit deviceRemoved();
        }
    }

    // Connect new devices.
    foreach (DeviceModel model, models)
    {
        if (!this->devices.contains(model.getName()))
        {
            QSharedPointer<CasparDevice> device(new CasparDevice(model.getAddress(), model.getPort()));

            this->deviceModels.insert(model.getName(), model);
            this->devices.insert(model.getName(), device);

            emit deviceAdded(*device);

            device->connectDevice();
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

const QSharedPointer<DeviceModel> DeviceManager::getDeviceModelByName(const QString& name) const
{
    foreach (const DeviceModel& model, this->deviceModels)
    {
        if (model.getName() == name)
            return QSharedPointer<DeviceModel>(new DeviceModel(model));
    }

    qWarning() << "No DeviceModel found for specified name";

    return QSharedPointer<DeviceModel>();
}

const QSharedPointer<DeviceModel> DeviceManager::getDeviceModelByAddress(const QString& address) const
{
    foreach (const DeviceModel& model, this->deviceModels)
    {
        if (model.getAddress() == address)
           return QSharedPointer<DeviceModel>(new DeviceModel(model));
    }

    qWarning() << "No DeviceModel found for specified address";

    return QSharedPointer<DeviceModel>();
}

const int DeviceManager::getDeviceCount() const
{
    return this->devices.count();
}

const QSharedPointer<CasparDevice> DeviceManager::getDeviceByName(const QString& name) const
{
    return this->devices.value(name);
}
