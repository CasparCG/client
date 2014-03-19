#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT ActiveRundownChangedEvent
{
    public:
        explicit ActiveRundownChangedEvent(const QString& path);

        const QString& getPath() const;

    private:
        QString path;
};
