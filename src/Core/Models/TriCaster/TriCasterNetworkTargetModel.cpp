#include "TriCasterNetworkTargetModel.h"

TriCasterNetworkTargetModel::TriCasterNetworkTargetModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterNetworkTargetModel::getId() const
{
    return this->id;
}

const QString& TriCasterNetworkTargetModel::getName() const
{
    return this->name;
}

const QString& TriCasterNetworkTargetModel::getValue() const
{
    return this->value;
}
