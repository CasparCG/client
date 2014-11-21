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
        void refresh();

        QList<DeviceModel> getDeviceModels() const;
        const QSharedPointer<DeviceModel> getDeviceModelByName(const QString& name) const;
        const QSharedPointer<DeviceModel> getDeviceModelByAddress(const QString& address) const;

        const int getDeviceCount() const;
        const QSharedPointer<CasparDevice> getDeviceByName(const QString& name) const;

        Q_SIGNAL void deviceRemoved();
        Q_SIGNAL void deviceAdded(CasparDevice&);

    private:
        QMap<QString, DeviceModel> deviceModels;
        QMap<QString, QSharedPointer<CasparDevice> > devices;
};

