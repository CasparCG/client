#include "AtemKeyerModel.h"

AtemKeyerModel::AtemKeyerModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemKeyerModel::getId() const
{
    return this->id;
}

const QString& AtemKeyerModel::getName() const
{
    return this->name;
}

const QString& AtemKeyerModel::getValue() const
{
    return this->value;
}
