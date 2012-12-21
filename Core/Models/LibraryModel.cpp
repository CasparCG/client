#include "LibraryModel.h"

LibraryModel::LibraryModel(int id, const QString& name, const QString& deviceName, const QString& type)
    : id(id), name(name), deviceName(deviceName), type(type)
{
}

int LibraryModel::getId() const
{
    return this->id;
}

const QString& LibraryModel::getName() const
{
    return this->name;
}

const QString& LibraryModel::getDeviceName() const
{
    return this->deviceName;
}

const QString& LibraryModel::getType() const
{
    return this->type;
}

void LibraryModel::setName(const QString& name)
{
    this->name = name;
}

void LibraryModel::setDeviceName(const QString& deviceName)
{
    this->deviceName = deviceName;
}
