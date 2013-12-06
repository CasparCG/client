#include "TriCasterDeviceManager.h"
#include "DatabaseManager.h"
#include "Events/ConnectionStateChangedEvent.h"
#include "Models/TriCaster/TriCasterDeviceModel.h"

#include <stdexcept>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(TriCasterDeviceManager, tricasterDeviceManager)

TriCasterDeviceManager::TriCasterDeviceManager()
{
}

TriCasterDeviceManager& TriCasterDeviceManager::getInstance()
{
    return *tricasterDeviceManager();
}

void TriCasterDeviceManager::initialize()
{
    QList<TriCasterDeviceModel> models = DatabaseManager::getInstance().getTriCasterDevice();
    foreach (const TriCasterDeviceModel& model, models)
    {
        QSharedPointer<TriCasterDevice> device(new TriCasterDevice(model.getAddress(), model.getPort()));

        this->deviceModels.insert(model.getName(), model);
        this->devices.insert(model.getName(), device);

        emit tricasterDeviceAdded(*device);

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

QList<TriCasterDeviceModel> TriCasterDeviceManager::getTriCasterDeviceModels() const
{
    QList<TriCasterDeviceModel> models;
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
        models.push_back(model);

    return models;
}

const TriCasterDeviceModel TriCasterDeviceManager::getTriCasterDeviceModelByName(const QString& name) const
{
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
    {
        if (model.getName() == name)
            return model;
    }

    qCritical() << "No TriCasterDeviceModel found for specified name";
}

const TriCasterDeviceModel TriCasterDeviceManager::getTriCasterDeviceModelByAddress(const QString& address) const
{
    foreach (const TriCasterDeviceModel& model, this->deviceModels)
    {
        if (model.getAddress() == address)
            return model;
    }

    qCritical() << "No TriCasterDeviceModel found for specified address";
}

const int TriCasterDeviceManager::getTriCasterDeviceCount() const
{
    return this->devices.count();
}

const QSharedPointer<TriCasterDevice> TriCasterDeviceManager::getTriCasterDeviceByName(const QString& name) const
{
    return this->devices.value(name);
}
