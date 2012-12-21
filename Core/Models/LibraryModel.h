#pragma once

#include "../Shared.h"

#include <QtCore/QObject>
#include <QtCore/QString>

class CORE_EXPORT LibraryModel
{
    public:
        explicit LibraryModel() { }
        explicit LibraryModel(int id, const QString& name, const QString& deviceName, const QString& type);

        int getId() const;
        const QString& getName() const;
        const QString& getDeviceName() const;
        const QString& getType() const;

        void setName(const QString& name);
        void setDeviceName(const QString& deviceName);

    private:
        int id;
        QString name;
        QString deviceName;
        QString type;
};
