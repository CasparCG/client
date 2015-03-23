#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT StatusbarEvent
{
    public:
        explicit StatusbarEvent(const QString& message, int timeout = 3000);

        int getTimeout() const;
        const QString& getMessage() const;

    private:
        int timeout;
        QString message;
};
