#include "AddTemplateDataEvent.h"

#include "Global.h"

AddTemplateDataEvent::AddTemplateDataEvent(const QString& value, bool storedData)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AddTemplateData)), value(value), storedData(storedData)
{
}

const QString& AddTemplateDataEvent::getValue()
{
    return this->value;
}

bool AddTemplateDataEvent::getStoredData()
{
    return this->storedData;
}
