#include "AtemSwitcherModel.h"

AtemSwitcherModel::AtemSwitcherModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemSwitcherModel::getId() const
{
    return this->id;
}

const QString& AtemSwitcherModel::getName() const
{
    return this->name;
}

const QString& AtemSwitcherModel::getValue() const
{
    return this->value;
}
