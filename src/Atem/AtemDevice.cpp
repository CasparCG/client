#include "AtemDevice.h"

#include <math.h>
#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

AtemDevice::AtemDevice(const QString& address, QObject* parent)
    : SwitcherDevice(address, parent)
{
}

void AtemDevice::triggerAuto(const QString& target, int speed, const QString& transition)
{
    SwitcherDevice::atemConnection->setTransitionType(transition.toInt());

    if (target == "background")
    {
        switch (transition.toInt())
        {
            case 0: // Mix
                SwitcherDevice::atemConnection->setMixFrames(speed);
            case 1: // Dip
                SwitcherDevice::atemConnection->setDipFrames(speed);
            case 2: // Wipe
                SwitcherDevice::atemConnection->setWipeFrames(speed);
            case 5: // DVE
                SwitcherDevice::atemConnection->setDVERate(speed);
        }

        SwitcherDevice::atemConnection->doAuto();
    }
    else
    {
        SwitcherDevice::atemConnection->setDownstreamKeyFrameRate(target.toInt(), speed);

        SwitcherDevice::atemConnection->doDownstreamKeyAuto(target.toInt());
    }
}

void AtemDevice::triggerCut()
{
    SwitcherDevice::atemConnection->doCut();
}

QMap<quint16, QAtemConnection::InputInfo> AtemDevice::inputInfos()
{
    return SwitcherDevice::atemConnection->inputInfos();
}

QHash<quint16, QAtemConnection::AudioInput> AtemDevice::audioInputs()
{
    return SwitcherDevice::atemConnection->audioInputs();
}

void AtemDevice::selectInput(const QString& switcher, const QString& input)
{
    if (switcher == "pgm")
        SwitcherDevice::atemConnection->changeProgramInput(input.toInt());
    else if (switcher == "prev")
        SwitcherDevice::atemConnection->changePreviewInput(input.toInt());
}

void AtemDevice::setKeyerState(const QString& keyer, bool state)
{
    if (keyer == "0" || keyer == "1") // Downstream keyer.
        SwitcherDevice::atemConnection->setDownstreamKeyOn(keyer.toInt(), state);
    else
        SwitcherDevice::atemConnection->setUpstreamKeyOn(keyer.toInt() - 2, state);
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
        case SwitcherDevice::CONNECTIONSTATE:
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
