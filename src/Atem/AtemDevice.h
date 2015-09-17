#pragma once

#include "Shared.h"

#include "qatemtypes.h"

#include "SwitcherDevice.h"

class ATEM_EXPORT AtemDevice : public SwitcherDevice
{
    Q_OBJECT

    public:
        explicit AtemDevice(const QString& address, QObject* parent = 0);

        quint8 mixerEffects();

        void toggleFadeToBlack(const QString& me);
        void triggerAuto(const QString& target, int speed, const QString& transition, const QString& me);
        void triggerCut(const QString& me);

        QVector<QAtem::MacroInfo> macroInfos();
        QMap<quint16, QAtem::InputInfo> inputInfos();
        QHash<quint16, QAtem::AudioInput> audioInputs();

        void setAuxSource(const QString& aux, const QString& source);
        void selectInput(const QString& switcher, const QString& input, const QString& me);
        void playMacro(const QString& preset);

        void setKeyerState(const QString& keyer, bool state, const QString& me);
        void setVideoFormat(const QString& format);
        void setAudioInputState(const QString& input, const QString& state);
        void setAudioInputBalance(const QString& input, float balance);
        void setAudioGain(const QString& source, float gain);

        Q_SIGNAL void connectionStateChanged(AtemDevice&);

    protected:
        void sendNotification();
};
