#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT ReloadRundownEvent
{
    public:
        explicit ReloadRundownEvent(const QString& path = "");

        const QString& getPath() const;

    private:
        QString path;
};
