#include "TriCasterDeviceManager.h"
#include "DatabaseManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Models/TriCaster/TriCasterDeviceModel.h"

#include <stdexcept>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(TriCasterDeviceManager, deviceManager)

TriCasterDeviceManager::TriCasterDeviceManager()
{
}

TriCasterDeviceManager& TriCasterDeviceManager::getInstance()
{
    return *deviceManager();
}

void TriCasterDeviceManager::initialize()
{
    QList<TriCasterDeviceModel> models = DatabaseManager::getInstance().getTriCasterDevice();
    foreach (const TriCasterDeviceModel& model, models)
    {
        QSharedPointer<TriCasterDevice> device(new TriCasterDevice(model.getAddress(), model.getPort()));

        this->deviceModels.insert(model.getName(), model);
        this->devices.insert(model.getName(), device);

        emit deviceAdded(*device);

        device->connectDevice();
    }
}

void TriCasterDeviceManager::uninitialize()
{
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<TriCasterDevice>& device = this->devices[key];
        device->disconnectDevice();
    }
}

void TriCasterDeviceManager::refresh()
{
    QList<TriCasterDeviceModel> models = DatabaseManager::getInstance().getTriCasterDevice();

    // Disconnect old devices.
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<TriCasterDevice>& device = this->devices[key];

        bool foundDevice = false;
        foreach (TriCasterDeviceModel model, models)
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
    foreach (TriCasterDeviceModel model, models)
    {
        if (!this->devices.contains(model.getName()))
        {
            QSharedPointer<TriCasterDevice> device(new TriCasterDevice(model.getAddress(), model.getPort()));

            this->deviceModels.insert(model.getName(), model);
            this->devices.insert(model.getName(), device);

            emit deviceAdded(*device);

            device->connectDevice();
        }
    }
}

QList<TriCasterDeviceModel> TriCasterDeviceManager::getDeviceModels() const
{
    QList<TriCasterDeviceModel> models;
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
        models.push_back(model);

    return models;
}

const TriCasterDeviceModel TriCasterDeviceManager::getDeviceModelByName(const QString& name) const
{
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
    {
        if (model.getName() == name)
            return model;
    }

    qCritical() << "No TriCasterDeviceModel found for specified name";
}

const TriCasterDeviceModel TriCasterDeviceManager::getDeviceModelByAddress(const QString& address) const
{
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
    {
        if (model.getAddress() == address)
            return model;
    }

    qCritical() << "No TriCasterDeviceModel found for specified address";
}

const int TriCasterDeviceManager::getDeviceCount() const
{
    return this->devices.count();
}

const QSharedPointer<TriCasterDevice> TriCasterDeviceManager::getDeviceByName(const QString& name) const
{
    return this->devices.value(name);
}
