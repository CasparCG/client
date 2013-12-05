#include "TriCasterSwitcherModel.h"

TriCasterSwitcherModel::TriCasterSwitcherModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterSwitcherModel::getId() const
{
    return this->id;
}

const QString& TriCasterSwitcherModel::getName() const
{
    return this->name;
}

const QString& TriCasterSwitcherModel::getValue() const
{
    return this->value;
}
