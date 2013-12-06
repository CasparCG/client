#pragma once

#include "Shared.h"

#include "NTFCDevice.h"

class TRICASTER_EXPORT TriCasterDevice : public NTFCDevice
{
    Q_OBJECT

    public:
        explicit TriCasterDevice(const QString& address, int port = 5950, QObject* parent = 0);

        void triggerAuto(const QString& target, const QString& speed, const QString& transition);
        void triggerTake(const QString& target);

        void selectInput(const QString& switcher, const QString& input);
        void selectPreset(const QString& source, const QString& preset);

        Q_SIGNAL void connectionStateChanged(TriCasterDevice&);
        Q_SIGNAL void responseChanged(const QString&, TriCasterDevice&);

    protected:
        void sendNotification();
};
