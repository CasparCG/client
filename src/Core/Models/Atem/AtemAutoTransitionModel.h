#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AtemAutoTransitionModel
{
    public:
        explicit AtemAutoTransitionModel(int id, const QString& name, const QString& value);

        int getId() const;
        const QString& getName() const;
        const QString& getValue() const;

    private:
        int id;
        QString name;
        QString value;
};


