#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT DirectionModel
{
    public:
        explicit DirectionModel(int id, const QString& value);

        int getId() const;
        const QString& getValue() const;

    private:
        int id;
        QString value;
};


