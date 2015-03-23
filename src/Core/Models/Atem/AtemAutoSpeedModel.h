#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AtemAutoSpeedModel
{
    public:
        explicit AtemAutoSpeedModel(int id, const QString& name, const QString& value, const QString& product);

        int getId() const;
        const QString& getName() const;
        const QString& getValue() const;
        const QString& getProduct() const;

    private:
        int id;
        QString name;
        QString value;
        QString product;
};


