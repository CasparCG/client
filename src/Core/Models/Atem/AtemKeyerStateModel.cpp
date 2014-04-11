#include "AtemKeyerStateModel.h"

AtemKeyerStateModel::AtemKeyerStateModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemKeyerStateModel::getId() const
{
    return this->id;
}

const QString& AtemKeyerStateModel::getName() const
{
    return this->name;
}

const QString& AtemKeyerStateModel::getValue() const
{
    return this->value;
}
