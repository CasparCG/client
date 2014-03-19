#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT OpenRundownEvent
{
    public:
        explicit OpenRundownEvent(const QString& path = "");

        const QString& getPath() const;

    private:
        QString path;
};
