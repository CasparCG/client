#include "TriCasterProductModel.h"

TriCasterProductModel::TriCasterProductModel(int id, const QString& name)
    : id(id), name(name)
{    
}

int TriCasterProductModel::getId() const
{
    return this->id;
}

const QString& TriCasterProductModel::getName() const
{
    return this->name;
}
