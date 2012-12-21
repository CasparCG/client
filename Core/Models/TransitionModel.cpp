#include "TransitionModel.h"

TransitionModel::TransitionModel(int id, const QString& value)
    : id(id), value(value)
{    
}

int TransitionModel::getId() const
{
    return this->id;
}

const QString& TransitionModel::getValue() const
{
    return this->value;
}
