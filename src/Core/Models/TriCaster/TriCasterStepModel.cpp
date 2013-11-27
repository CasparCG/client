#include "TriCasterStepModel.h"

TriCasterStepModel::TriCasterStepModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
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
