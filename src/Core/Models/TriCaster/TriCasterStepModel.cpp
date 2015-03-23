#include "TriCasterStepModel.h"

TriCasterStepModel::TriCasterStepModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int TriCasterStepModel::getId() const
{
    return this->id;
}

const QString& TriCasterStepModel::getName() const
{
    return this->name;
}

const QString& TriCasterStepModel::getValue() const
{
    return this->value;
}

const QString& TriCasterStepModel::getProduct() const
{
    return this->product;
}
