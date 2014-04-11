#pragma once

#include "Shared.h"
#include "Models/Atem/AtemDeviceModel.h"

#include "AtemDevice.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class CORE_EXPORT AtemDeviceManager : public QObject
{
    Q_OBJECT

    public:
        explicit AtemDeviceManager();

        static AtemDeviceManager& getInstance();

        void initialize();
        void uninitialize();
        void refresh();

        QList<AtemDeviceModel> getDeviceModels() const;
        const AtemDeviceModel getDeviceModelByName(const QString& name) const;
        const AtemDeviceModel getDeviceModelByAddress(const QString& address) const;

        const int getDeviceCount() const;
        const QSharedPointer<AtemDevice> getDeviceByName(const QString& name) const;

        const QSharedPointer<AtemDevice> getDevice() const;

        Q_SIGNAL void deviceRemoved();
        Q_SIGNAL void deviceAdded(AtemDevice&);

    private:
        QMap<QString, AtemDeviceModel> deviceModels;
        QMap<QString, QSharedPointer<AtemDevice> > devices;
};

