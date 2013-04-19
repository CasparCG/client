#include "DirectionModel.h"

DirectionModel::DirectionModel(int id, const QString& value)
    : id(id), value(value)
{    
}

int DirectionModel::getId() const
{
    return this->id;
}

const QString& DirectionModel::getValue() const
{
    return this->value;
}
