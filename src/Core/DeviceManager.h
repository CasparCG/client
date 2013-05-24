#pragma once

#include "Shared.h"
#include "Models/DeviceModel.h"

#include "CasparDevice.h"
#include "OscReceiver.h"

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
        const DeviceModel getDeviceModelByAddress(const QString& address) const;

        const int getDeviceCount() const;
        const QSharedPointer<CasparDevice> getDeviceByName(const QString& name) const;

        const OscReceiver* getOscReceiver() const;

        Q_SIGNAL void deviceRemoved();
        Q_SIGNAL void deviceAdded(CasparDevice&);

    private:
        OscReceiver* oscReceiver;
        QMap<QString, DeviceModel> deviceModels;
        QMap<QString, QSharedPointer<CasparDevice> > devices;
};

