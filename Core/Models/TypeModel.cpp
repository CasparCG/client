#include "TypeModel.h"

TypeModel::TypeModel(int id, const QString& name)
    : id(id), name(name)
{    
}

int TypeModel::getId() const
{
    return this->id;
}

const QString& TypeModel::getName() const
{
    return this->name;
}
