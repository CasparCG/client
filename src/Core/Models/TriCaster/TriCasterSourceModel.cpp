#include "TriCasterSourceModel.h"

TriCasterSourceModel::TriCasterSourceModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterSourceModel::getId() const
{
    return this->id;
}

const QString& TriCasterSourceModel::getName() const
{
    return this->name;
}

const QString& TriCasterSourceModel::getValue() const
{
    return this->value;
}
