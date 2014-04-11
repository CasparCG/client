#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AtemDeviceModel
{
    public:
        explicit AtemDeviceModel(int id, const QString& name, const QString& address, const QString& description);

        int getId() const;
        const QString& getName() const;
        const QString& getAddress() const;
        const QString& getDescription() const;

    private:
        int id;
        QString name;
        QString address;
        QString description;
};
