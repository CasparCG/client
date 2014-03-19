#include "AddTemplateDataEvent.h"

#include "Global.h"

AddTemplateDataEvent::AddTemplateDataEvent(const QString& value, bool storedData)
    : value(value), storedData(storedData)
{
}

const QString& AddTemplateDataEvent::getValue() const
{
    return this->value;
}

bool AddTemplateDataEvent::getStoredData() const
{
    return this->storedData;
}
