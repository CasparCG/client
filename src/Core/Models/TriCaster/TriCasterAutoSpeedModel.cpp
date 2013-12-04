#include "TriCasterAutoSpeedModel.h"

TriCasterAutoSpeedModel::TriCasterAutoSpeedModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterAutoSpeedModel::getId() const
{
    return this->id;
}

const QString& TriCasterAutoSpeedModel::getName() const
{
    return this->name;
}

const QString& TriCasterAutoSpeedModel::getValue() const
{
    return this->value;
}
