#include "TriCasterAutoSpeedModel.h"

TriCasterAutoSpeedModel::TriCasterAutoSpeedModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int TriCasterAutoSpeedModel::getId() const
{
    return this->id;
}

const QString& TriCasterAutoSpeedModel::getName() const
{
    return this->name;
}

const QString& TriCasterAutoSpeedModel::getValue() const
{
    return this->value;
}

const QString& TriCasterAutoSpeedModel::getProduct() const
{
    return this->product;
}
