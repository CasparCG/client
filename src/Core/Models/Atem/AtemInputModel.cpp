#include "AtemInputModel.h"

AtemInputModel::AtemInputModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemInputModel::getId() const
{
    return this->id;
}

const QString& AtemInputModel::getName() const
{
    return this->name;
}

const QString& AtemInputModel::getValue() const
{
    return this->value;
}
