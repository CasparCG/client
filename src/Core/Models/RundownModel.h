#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT RundownModel
{
    public:
        explicit RundownModel(int id, const QString& page, const QString& name, const QString& deviceName, const QString& type);

        int getId() const;
        const QString& getPage() const;
        const QString& getName() const;
        const QString& getDeviceName() const;
        const QString& getType() const;

    private:
        int id;
        QString page;
        QString name;
        QString deviceName;
        QString type;
};
