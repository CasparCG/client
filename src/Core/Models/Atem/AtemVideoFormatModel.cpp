#include "AtemVideoFormatModel.h"

AtemVideoFormatModel::AtemVideoFormatModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemVideoFormatModel::getId() const
{
    return this->id;
}

const QString& AtemVideoFormatModel::getName() const
{
    return this->name;
}

const QString& AtemVideoFormatModel::getValue() const
{
    return this->value;
}
