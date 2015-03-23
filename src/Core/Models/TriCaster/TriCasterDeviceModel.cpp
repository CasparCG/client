#include "TriCasterDeviceModel.h"

TriCasterDeviceModel::TriCasterDeviceModel(int id, const QString& name, const QString& address, int port, const QString& description)
    : id(id), port(port), name(name), address(address), description(description)
{
}

int TriCasterDeviceModel::getId() const
{
    return this->id;
}

const QString& TriCasterDeviceModel::getName() const
{
    return this->name;
}

const QString& TriCasterDeviceModel::getAddress() const
{
    return this->address;
}

int TriCasterDeviceModel::getPort() const
{
    return this->port;
}

const QString& TriCasterDeviceModel::getDescription() const
{
    return this->description;
}
