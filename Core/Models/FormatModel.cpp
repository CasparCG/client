#include "FormatModel.h"

FormatModel::FormatModel(int id, const QString& value)
    : id(id), value(value)
{    
}

int FormatModel::getId() const
{
    return this->id;
}

const QString& FormatModel::getValue() const
{
    return this->value;
}
