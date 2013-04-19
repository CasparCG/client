#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT BlendModeModel
{
    public:
        explicit BlendModeModel(int id, const QString& value);

        int getId() const;
        const QString& getValue() const;

    private:
        int id;
        QString value;
};


