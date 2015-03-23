#include "AtemStepModel.h"

AtemStepModel::AtemStepModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemStepModel::getId() const
{
    return this->id;
}

const QString& AtemStepModel::getName() const
{
    return this->name;
}

const QString& AtemStepModel::getValue() const
{
    return this->value;
}
