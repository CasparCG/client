#include "TriCasterPresetModel.h"

TriCasterPresetModel::TriCasterPresetModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
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
