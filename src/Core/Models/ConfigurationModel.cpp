#include "ConfigurationModel.h"

ConfigurationModel::ConfigurationModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{
}

int ConfigurationModel::getId() const
{
    return this->id;
}

const QString& ConfigurationModel::getName() const
{
    return this->name;
}

const QString& ConfigurationModel::getValue() const
{
    return this->value;
}
