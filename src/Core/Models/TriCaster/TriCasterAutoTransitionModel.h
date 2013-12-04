#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TriCasterAutoTransitionModel
{
    public:
        explicit TriCasterAutoTransitionModel(int id, const QString& name, const QString& value);

        int getId() const;
        const QString& getName() const;
        const QString& getValue() const;

    private:
        int id;
        QString name;
        QString value;
};


