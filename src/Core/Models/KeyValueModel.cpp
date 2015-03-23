#include "KeyValueModel.h"

KeyValueModel::KeyValueModel(const QString& key, const QString& value)
{
    this->key = key;
    this->value = value;
}

const QString& KeyValueModel::getKey() const
{
    return this->key;
}

const QString& KeyValueModel::getValue() const
{
    return this->value;
}

void KeyValueModel::setKey(const QString& key)
{
    this->key = key;
}

void KeyValueModel::setValue(const QString& value)
{
    this->value = value;
}
