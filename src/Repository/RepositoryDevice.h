#pragma once

#include "Shared.h"

#include "RrupDevice.h"
#include "Models/RepositoryAdd.h"
#include "Models/RepositoryRemove.h"

class REPOSITORY_EXPORT RepositoryDevice : public RrupDevice
{
    Q_OBJECT

    public:
        explicit RepositoryDevice(const QString& address, int port = 8250, QObject* parent = 0);

        const QString resolveIpAddress() const;

        void subscribe(const QString& name);

        Q_SIGNAL void connectionStateChanged(RepositoryDevice&);
        Q_SIGNAL void addChanged(const RepositoryAdd&, RepositoryDevice&);
        Q_SIGNAL void removeChanged(const RepositoryRemove&, RepositoryDevice&);

    protected:
        void sendNotification();
};
