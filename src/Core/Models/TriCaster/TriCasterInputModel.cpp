#include "TriCasterInputModel.h"

TriCasterInputModel::TriCasterInputModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int TriCasterInputModel::getId() const
{
    return this->id;
}

const QString& TriCasterInputModel::getName() const
{
    return this->name;
}

const QString& TriCasterInputModel::getValue() const
{
    return this->value;
}
