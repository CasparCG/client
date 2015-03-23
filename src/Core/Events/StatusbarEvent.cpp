#include "StatusbarEvent.h"

#include "Global.h"

StatusbarEvent::StatusbarEvent(const QString& message, int timeout)
    : timeout(timeout), message(message)
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
