#pragma once

#include "Shared.h"
#include "Models/DeviceModel.h"

#include "CasparDevice.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class CORE_EXPORT DeviceManager : public QObject
{
    Q_OBJECT

    public:
        explicit DeviceManager();

        static DeviceManager& getInstance();

        void initialize();
        void uninitialize();
        void synchronize();

        QList<DeviceModel> getDeviceModels() const;
        const DeviceModel getDeviceModelByAddress(const QString& address) const;

        const int getConnectionCount() const;
        const QSharedPointer<CasparDevice> getConnectionByName(const QString& name) const;

        Q_SIGNAL void deviceRemoved();
        Q_SIGNAL void deviceAdded(CasparDevice&);

    private:
        QMap<QString, DeviceModel> deviceModels;
        QMap<QString, QSharedPointer<CasparDevice> > connections;

        Q_SLOT void deviceConnectionStateChanged(CasparDevice&);
};

