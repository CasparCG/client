#include "TriCasterSwitcherModel.h"

TriCasterSwitcherModel::TriCasterSwitcherModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
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

const QString& TriCasterSwitcherModel::getProduct() const
{
    return this->product;
}
