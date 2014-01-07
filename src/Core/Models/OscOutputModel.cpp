#include "OscOutputModel.h"

OscOutputModel::OscOutputModel(int id, const QString& name, const QString& address, int port, const QString& description)
    : id(id), port(port), name(name), address(address), description(description)
{
}

int OscOutputModel::getId() const
{
    return this->id;
}

const QString& OscOutputModel::getName() const
{
    return this->name;
}

const QString& OscOutputModel::getAddress() const
{
    return this->address;
}

int OscOutputModel::getPort() const
{
    return this->port;
}

const QString& OscOutputModel::getDescription() const
{
    return this->description;
}
