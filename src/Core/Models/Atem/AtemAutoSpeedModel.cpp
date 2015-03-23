#include "AtemAutoSpeedModel.h"

AtemAutoSpeedModel::AtemAutoSpeedModel(int id, const QString& name, const QString& value, const QString& product)
    : id(id), name(name), value(value), product(product)
{    
}

int AtemAutoSpeedModel::getId() const
{
    return this->id;
}

const QString& AtemAutoSpeedModel::getName() const
{
    return this->name;
}

const QString& AtemAutoSpeedModel::getValue() const
{
    return this->value;
}

const QString& AtemAutoSpeedModel::getProduct() const
{
    return this->product;
}
