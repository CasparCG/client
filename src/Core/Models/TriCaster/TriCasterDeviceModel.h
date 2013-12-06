#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TriCasterDeviceModel
{
    public:
        explicit TriCasterDeviceModel(int id, const QString& name, const QString& address, int port, const QString& description);

        int getId() const;
        int getPort() const;
        const QString& getName() const;
        const QString& getAddress() const;
        const QString& getDescription() const;

    private:
        int id;
        int port;
        QString name;
        QString address;
        QString description;
};
