#include "AtemAudioInputStateModel.h"

AtemAudioInputStateModel::AtemAudioInputStateModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{    
}

int AtemAudioInputStateModel::getId() const
{
    return this->id;
}

const QString& AtemAudioInputStateModel::getName() const
{
    return this->name;
}

const QString& AtemAudioInputStateModel::getValue() const
{
    return this->value;
}
