#pragma once

#include "Shared.h"

#include "qatemconnection.h"

#include "SwitcherDevice.h"

class ATEM_EXPORT AtemDevice : public SwitcherDevice
{
    Q_OBJECT

    public:
        explicit AtemDevice(const QString& address, QObject* parent = 0);

        void triggerAuto(const QString& target, int speed, const QString& transition);
        void triggerCut();

        QMap<quint16, QAtemConnection::InputInfo> inputInfos();
        QHash<quint16, QAtemConnection::AudioInput> audioInputs();

        void selectInput(const QString& switcher, const QString& input);

        void setKeyerState(const QString& keyer, bool state);
        void setVideoFormat(const QString& format);
        void setAudioInputState(const QString& input, const QString& state);
        void setAudioInputBalance(const QString& input, float balance);
        void setAudioGain(const QString& source, float gain);

        Q_SIGNAL void connectionStateChanged(AtemDevice&);

    protected:
        void sendNotification();
};
