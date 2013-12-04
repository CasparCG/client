#include "TriCasterAutoTransitionModel.h"

TriCasterAutoTransitionModel::TriCasterAutoTransitionModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
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
