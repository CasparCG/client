#include "TriCasterPresetModel.h"

TriCasterPresetModel::TriCasterPresetModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int TriCasterPresetModel::getId() const
{
    return this->id;
}

const QString& TriCasterPresetModel::getName() const
{
    return this->name;
}

const QString& TriCasterPresetModel::getValue() const
{
    return this->value;
}

const QString& TriCasterPresetModel::getProduct() const
{
    return this->product;
}
