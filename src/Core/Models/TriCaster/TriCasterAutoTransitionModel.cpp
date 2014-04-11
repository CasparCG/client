#include "TriCasterAutoTransitionModel.h"

TriCasterAutoTransitionModel::TriCasterAutoTransitionModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int TriCasterAutoTransitionModel::getId() const
{
    return this->id;
}

const QString& TriCasterAutoTransitionModel::getName() const
{
    return this->name;
}

const QString& TriCasterAutoTransitionModel::getValue() const
{
    return this->value;
}

const QString& TriCasterAutoTransitionModel::getProduct() const
{
    return this->product;
}
