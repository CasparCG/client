#pragma once

#include "Shared.h"

#include <QtCore/QString>

class COMMON_EXPORT Xml
{
    public:
        static QString encode(const QString& data);
        static QString decode(const QString& data);

    private:
        Xml() {}
};
