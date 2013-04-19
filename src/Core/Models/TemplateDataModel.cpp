#include "TemplateDataModel.h"

TemplateDataModel::TemplateDataModel(const QString& key, const QString& value)
{
    this->key = key;
    this->value = value;
}

const QString &TemplateDataModel::getKey() const
{
    return this->key;
}

const QString &TemplateDataModel::getValue() const
{
    return this->value;
}

void TemplateDataModel::setKey(const QString& key)
{
    this->key = key;
}

void TemplateDataModel::setValue(const QString& value)
{
    this->value = value;
}
