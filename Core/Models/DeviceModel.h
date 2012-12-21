#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT DeviceModel
{
    public:
        explicit DeviceModel(int id, const QString& name, const QString& address, int port, const QString& username,
                             const QString& password, const QString& description, const QString& version, const QString& shadow);

        int getId() const;
        int getPort() const;
        const QString& getName() const;
        const QString& getAddress() const;
        const QString& getUsername() const;
        const QString& getPassword() const;
        const QString& getDescription() const;
        const QString& getVersion() const;
        const QString& getShadow() const;

    private:
        int id;
        int port;
        QString name;
        QString address;
        QString username;
        QString password;
        QString description;
        QString version;
        QString shadow;
};
