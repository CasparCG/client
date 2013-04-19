#include "RundownModel.h"

RundownModel::RundownModel(int id, const QString& page, const QString& name, const QString& deviceName, const QString& type)
    : id(id), page(page), name(name), deviceName(deviceName), type(type)
{
}

int RundownModel::getId() const
{
    return this->id;
}

const QString& RundownModel::getPage() const
{
    return this->page;
}

const QString& RundownModel::getName() const
{
    return this->name;
}

const QString& RundownModel::getDeviceName() const
{
    return this->deviceName;
}

const QString& RundownModel::getType() const
{
    return this->type;
}
