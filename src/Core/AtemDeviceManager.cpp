#include "AtemDeviceManager.h"
#include "DatabaseManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Models/Atem/AtemDeviceModel.h"

#include <stdexcept>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(AtemDeviceManager, atemDeviceManager)

AtemDeviceManager::AtemDeviceManager()
{
}

AtemDeviceManager& AtemDeviceManager::getInstance()
{
    return *atemDeviceManager();
}

void AtemDeviceManager::initialize()
{
    QList<AtemDeviceModel> models = DatabaseManager::getInstance().getAtemDevice();
    foreach (const AtemDeviceModel& model, models)
    {
        QSharedPointer<AtemDevice> device(new AtemDevice(model.getAddress()));

        this->deviceModels.insert(model.getName(), model);
        this->devices.insert(model.getName(), device);

        emit deviceAdded(*device);

        device->connectDevice();
    }
}

void AtemDeviceManager::uninitialize()
{
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<AtemDevice>& device = this->devices[key];
        device->disconnectDevice();
    }
}

void AtemDeviceManager::refresh()
{
    QList<AtemDeviceModel> models = DatabaseManager::getInstance().getAtemDevice();

    // Disconnect old devices.
    foreach (const QString& key, this->devices.keys())
    {
        QSharedPointer<AtemDevice>& device = this->devices[key];

        bool foundDevice = false;
        foreach (AtemDeviceModel model, models)
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
    foreach (AtemDeviceModel model, models)
    {
        if (!this->devices.contains(model.getName()))
        {
            QSharedPointer<AtemDevice> device(new AtemDevice(model.getAddress()));

            this->deviceModels.insert(model.getName(), model);
            this->devices.insert(model.getName(), device);

            emit deviceAdded(*device);

            device->connectDevice();
        }
    }
}

QList<AtemDeviceModel> AtemDeviceManager::getDeviceModels() const
{
    QList<AtemDeviceModel> models;
    foreach (const AtemDeviceModel& model, this->deviceModels)
        models.push_back(model);

    return models;
}

const AtemDeviceModel AtemDeviceManager::getDeviceModelByName(const QString& name) const
{
    foreach (const AtemDeviceModel& model, this->deviceModels)
    {
        if (model.getName() == name)
            return model;
    }

    qCritical() << "No AtemDeviceModel found for specified name";
}

const AtemDeviceModel AtemDeviceManager::getDeviceModelByAddress(const QString& address) const
{
    foreach (const AtemDeviceModel& model, this->deviceModels)
    {
        if (model.getAddress() == address)
            return model;
    }

    qCritical() << "No TriCasterDeviceModel found for specified address";
}

const int AtemDeviceManager::getDeviceCount() const
{
    return this->devices.count();
}

const QSharedPointer<AtemDevice> AtemDeviceManager::getDeviceByName(const QString& name) const
{
    return this->devices.value(name);
}
