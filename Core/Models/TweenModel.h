#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TweenModel
{
    public:
        explicit TweenModel(int id, const QString& value);

        int getId() const;
        const QString& getValue() const;

    private:
        int id;
        QString value;
};


