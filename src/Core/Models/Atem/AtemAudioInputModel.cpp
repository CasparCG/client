#include "AtemAudioInputModel.h"

AtemAudioInputModel::AtemAudioInputModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemAudioInputModel::getId() const
{
    return this->id;
}

const QString& AtemAudioInputModel::getName() const
{
    return this->name;
}

const QString& AtemAudioInputModel::getValue() const
{
    return this->value;
}
