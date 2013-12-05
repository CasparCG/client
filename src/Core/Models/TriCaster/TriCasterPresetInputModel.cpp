#include "TriCasterPresetInputModel.h"

TriCasterPresetInputModel::TriCasterPresetInputModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterPresetInputModel::getId() const
{
    return this->id;
}

const QString& TriCasterPresetInputModel::getName() const
{
    return this->name;
}

const QString& TriCasterPresetInputModel::getValue() const
{
    return this->value;
}
