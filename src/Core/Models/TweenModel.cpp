#include "TweenModel.h"

TweenModel::TweenModel(int id, const QString& value)
    : id(id), value(value)
{    
}

int TweenModel::getId() const
{
    return this->id;
}

const QString& TweenModel::getValue() const
{
    return this->value;
}
