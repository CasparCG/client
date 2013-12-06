#pragma once

#include "Shared.h"
#include "Models/TriCaster/TriCasterDeviceModel.h"

#include "TriCasterDevice.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class CORE_EXPORT TriCasterDeviceManager : public QObject
{
    Q_OBJECT

    public:
        explicit TriCasterDeviceManager();

        static TriCasterDeviceManager& getInstance();

        void initialize();
        void uninitialize();
        void refresh();

        QList<TriCasterDeviceModel> getDeviceModels() const;
        const TriCasterDeviceModel getDeviceModelByName(const QString& name) const;
        const TriCasterDeviceModel getDeviceModelByAddress(const QString& address) const;

        const int getDeviceCount() const;
        const QSharedPointer<TriCasterDevice> getDeviceByName(const QString& name) const;

        const QSharedPointer<TriCasterDevice> getDevice() const;

        Q_SIGNAL void deviceRemoved();
        Q_SIGNAL void deviceAdded(TriCasterDevice&);

    private:
        QMap<QString, TriCasterDeviceModel> deviceModels;
        QMap<QString, QSharedPointer<TriCasterDevice> > devices;
};

