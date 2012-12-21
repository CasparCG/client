#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TransitionModel
{
    public:
        explicit TransitionModel(int id, const QString& value);

        int getId() const;
        const QString& getValue() const;

    private:
        int id;
        QString value;
};


