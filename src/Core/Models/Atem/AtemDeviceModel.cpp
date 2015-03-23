#include "AtemDeviceModel.h"

AtemDeviceModel::AtemDeviceModel(int id, const QString& name, const QString& address, const QString& description)
    : id(id), name(name), address(address), description(description)
{
}

int AtemDeviceModel::getId() const
{
    return this->id;
}

const QString& AtemDeviceModel::getName() const
{
    return this->name;
}

const QString& AtemDeviceModel::getAddress() const
{
    return this->address;
}

const QString& AtemDeviceModel::getDescription() const
{
    return this->description;
}
