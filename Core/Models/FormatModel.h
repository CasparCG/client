#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT FormatModel
{
    public:
        explicit FormatModel(int id, const QString& value);

        int getId() const;
        const QString& getValue() const;

    private:
        int id;
        QString value;
};


