#pragma once

#include "Shared.h"

#include "RrupDevice.h"
#include "Models/RepositoryChangeModel.h"

class REPOSITORY_EXPORT RepositoryDevice : public RrupDevice
{
    Q_OBJECT

    public:
        explicit RepositoryDevice(const QString& address, int port = 8250, QObject* parent = 0);

        const QString resolveIpAddress() const;

        void subscribe(const QString& rundown, const QString& profile);

        Q_SIGNAL void connectionStateChanged(RepositoryDevice&);
        Q_SIGNAL void repositoryChanged(const RepositoryChangeModel&, RepositoryDevice&);

    protected:
        void sendNotification();
};
