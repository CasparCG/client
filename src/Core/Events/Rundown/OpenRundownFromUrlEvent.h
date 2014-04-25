#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT OpenRundownFromUrlEvent
{
    public:
        explicit OpenRundownFromUrlEvent(const QString& path = "");

        const QString& getPath() const;

    private:
        QString path;
};
