#include "PresetModel.h"

PresetModel::PresetModel(int id, const QString& name, const QString& value)
    : id(id), name(name), value(value)
{
}

int PresetModel::getId() const
{
    return this->id;
}

const QString& PresetModel::getName() const
{
    return this->name;
}

const QString& PresetModel::getValue() const
{
    return this->value;
}
