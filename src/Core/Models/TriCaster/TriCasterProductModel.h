#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TriCasterProductModel
{
    public:
        explicit TriCasterProductModel(int id, const QString& name);

        int getId() const;
        const QString& getName() const;

    private:
        int id;
        QString name;
};


