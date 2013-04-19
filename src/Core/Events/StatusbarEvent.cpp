#include "StatusbarEvent.h"

#include "Global.h"

StatusbarEvent::StatusbarEvent(const QString& message, int timeout)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::StatusbarMessage)), timeout(timeout), message(message)
{
}

const QString& StatusbarEvent::getMessage() const
{
    return this->message;
}

int StatusbarEvent::getTimeout() const
{
    return this->timeout;
}
