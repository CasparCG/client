#include "AtemDevice.h"

#include "qatemmixeffect.h"
#include "qatemconnection.h"

#include <math.h>
#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

AtemDevice::AtemDevice(const QString& address, QObject* parent)
    : SwitcherDevice(address, parent)
{
}

void AtemDevice::triggerAuto(const QString& target, int speed, const QString& transition, const QString& me)
{
    SwitcherDevice::atemConnection->mixEffect(me.toInt())->setTransitionType(transition.toInt());

    if (target == "background")
    {
        switch (transition.toInt())
        {
            case 0: // Mix
                SwitcherDevice::atemConnection->mixEffect(me.toInt())->setMixFrames(speed);
            case 1: // Dip
                SwitcherDevice::atemConnection->mixEffect(me.toInt())->setDipFrames(speed);
            case 2: // Wipe
                SwitcherDevice::atemConnection->mixEffect(me.toInt())->setWipeFrames(speed);
            case 5: // DVE
                SwitcherDevice::atemConnection->mixEffect(me.toInt())->setDVERate(speed);
        }

        SwitcherDevice::atemConnection->mixEffect(me.toInt())->autoTransition();
    }
    else
    {
        SwitcherDevice::atemConnection->setDownstreamKeyFrameRate(target.toInt(), speed);

        SwitcherDevice::atemConnection->doDownstreamKeyAuto(target.toInt());
    }
}

void AtemDevice::triggerCut(const QString& me)
{
    SwitcherDevice::atemConnection->mixEffect(me.toInt())->cut();
}

QVector<QAtem::MacroInfo> AtemDevice::macroInfos()
{
    /*
    QMap<quint8, QAtem::InputInfo> macros = SwitcherDevice::atemConnection->macroInfos();
    foreach (quint8 key, macros.keys())
        qDebug() << "Name: " << macros.value(key).name << " Index: " << macros.value(key).index;
    */

    return SwitcherDevice::atemConnection->macroInfos();
}

QMap<quint16, QAtem::InputInfo> AtemDevice::inputInfos()
{
    /*
    QMap<quint16, QAtem::InputInfo> infos = SwitcherDevice::atemConnection->inputInfos();
    foreach (quint16 key, infos.keys())
        qDebug() << "LongText: " << infos.value(key).longText << ", ShortText: " << infos.value(key).shortText << " Index: " << infos.value(key).index << ", Internal Type: " << infos.value(key).internalType;
    */

    return SwitcherDevice::atemConnection->inputInfos();
}

QHash<quint16, QAtem::AudioInput> AtemDevice::audioInputs()
{
    return SwitcherDevice::atemConnection->audioInputs();
}

void AtemDevice::selectInput(const QString& switcher, const QString& input, const QString& me)
{
    if (switcher == "pgm")
        SwitcherDevice::atemConnection->mixEffect(me.toInt())->changeProgramInput(input.toInt());
    else if (switcher == "prev")
        SwitcherDevice::atemConnection->mixEffect(me.toInt())->changePreviewInput(input.toInt());
}

void AtemDevice::playMacro(const QString& preset)
{
    SwitcherDevice::atemConnection->runMacro(preset.toInt());
}

void AtemDevice::setAuxSource(const QString& aux, const QString& source)
{
    SwitcherDevice::atemConnection->setAuxSource(aux.toInt() - 8001, source.toInt());
}

void AtemDevice::setKeyerState(const QString& keyer, bool state)
{
    if (keyer == "0" || keyer == "1") // Downstream keyer.
        SwitcherDevice::atemConnection->setDownstreamKeyOn(keyer.toInt(), state);
    else
        SwitcherDevice::atemConnection->mixEffect(1)->setUpstreamKeyOnAir(keyer.toInt() - 2, state);
}

void AtemDevice::setAudioInputState(const QString& input, const QString& state)
{
    SwitcherDevice::atemConnection->setAudioInputState(input.toInt(), state.toInt());
}

void AtemDevice::setAudioInputBalance(const QString& input, float balance)
{
    SwitcherDevice::atemConnection->setAudioInputBalance(input.toInt(), balance);
}

void AtemDevice::setAudioGain(const QString& source, float gain)
{
    if (source == "0") // Master output.
        SwitcherDevice::atemConnection->setAudioMasterOutputGain(gain);
    else
        SwitcherDevice::atemConnection->setAudioInputGain(source.toInt(), gain);
}

void AtemDevice::setVideoFormat(const QString& format)
{
    SwitcherDevice::atemConnection->setVideoFormat(format.toInt());
}

void AtemDevice::sendNotification()
{
    switch (SwitcherDevice::command)
    {
        case SwitcherDevice::SwitcherDeviceCommand::CONNECTIONSTATE:
        {
            emit connectionStateChanged(*this);

            break;
        }
        default:
        {
            break;
        }
    }

    resetDevice();
}
