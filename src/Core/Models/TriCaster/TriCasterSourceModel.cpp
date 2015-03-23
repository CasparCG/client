#include "TriCasterSourceModel.h"

TriCasterSourceModel::TriCasterSourceModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int TriCasterSourceModel::getId() const
{
    return this->id;
}

const QString& TriCasterSourceModel::getName() const
{
    return this->name;
}

const QString& TriCasterSourceModel::getValue() const
{
    return this->value;
}

const QString& TriCasterSourceModel::getProduct() const
{
    return this->product;
}
