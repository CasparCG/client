#pragma once

#include "Shared.h"

#include "NtfcDevice.h"

class TRICASTER_EXPORT TriCasterDevice : public NtfcDevice
{
    Q_OBJECT

    public:
        explicit TriCasterDevice(const QString& address, int port = 5950, QObject* parent = 0);

        void triggerAuto(const QString& target, const QString& speed, const QString& transition);
        void triggerTake(const QString& target);

        void selectInput(const QString& switcher, const QString& input, const QString& product);
        void selectPreset(const QString& source, const QString& preset);

        void selectNetworkSource(const QString& target, const QString& source);

        void playMacro(const QString& name);

        Q_SIGNAL void connectionStateChanged(TriCasterDevice&);

    protected:
        void sendNotification();
};
