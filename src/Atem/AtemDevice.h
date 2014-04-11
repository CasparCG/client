#pragma once

#include "Shared.h"

#include "SwitcherDevice.h"

class ATEM_EXPORT AtemDevice : public SwitcherDevice
{
    Q_OBJECT

    public:
        explicit AtemDevice(const QString& address, QObject* parent = 0);

        void triggerAuto(const QString& target, int speed, const QString& transition);
        void triggerCut();

        void selectInput(const QString& switcher, const QString& input);

        void setKeyerState(const QString& keyer, bool state);
        void setVideoFormat(const QString& format);

        Q_SIGNAL void connectionStateChanged(AtemDevice&);

    protected:
        void sendNotification();
};
