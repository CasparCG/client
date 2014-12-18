#include "AtemMixerStepModel.h"

AtemMixerStepModel::AtemMixerStepModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemMixerStepModel::getId() const
{
    return this->id;
}

const QString& AtemMixerStepModel::getName() const
{
    return this->name;
}

const QString& AtemMixerStepModel::getValue() const
{
    return this->value;
}
