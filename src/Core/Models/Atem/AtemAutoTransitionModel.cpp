#include "AtemAutoTransitionModel.h"

AtemAutoTransitionModel::AtemAutoTransitionModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemAutoTransitionModel::getId() const
{
    return this->id;
}

const QString& AtemAutoTransitionModel::getName() const
{
    return this->name;
}

const QString& AtemAutoTransitionModel::getValue() const
{
    return this->value;
}
