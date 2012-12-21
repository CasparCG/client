#include "GpiModel.h"

GpiModel::GpiModel(int id, const QString& trigger, const QString& action)
    : id(id), trigger(trigger), action(action)
{    
}

int GpiModel::getId() const
{
    return this->id;
}

const QString& GpiModel::getTrigger() const
{
    return this->trigger;
}

const QString& GpiModel::getAction() const
{
    return this->action;
}
