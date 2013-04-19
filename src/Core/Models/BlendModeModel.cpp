#include "BlendModeModel.h"

BlendModeModel::BlendModeModel(int id, const QString& value) :
    id(id), value(value)
{    
}

int BlendModeModel::getId() const
{
    return this->id;
}

const QString& BlendModeModel::getValue() const
{
    return this->value;
}
