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

        QList<TriCasterDeviceModel> getTriCasterDeviceModels() const;
        const TriCasterDeviceModel getTriCasterDeviceModelByName(const QString& name) const;
        const TriCasterDeviceModel getTriCasterDeviceModelByAddress(const QString& address) const;

        const int getTriCasterDeviceCount() const;
        const QSharedPointer<TriCasterDevice> getTriCasterDeviceByName(const QString& name) const;

        const QSharedPointer<TriCasterDevice> getTriCasterDevice() const;

        Q_SIGNAL void tricasterDeviceRemoved();
        Q_SIGNAL void tricasterDeviceAdded(TriCasterDevice&);

    private:
        QMap<QString, TriCasterDeviceModel> deviceModels;
        QMap<QString, QSharedPointer<TriCasterDevice> > devices;
};

