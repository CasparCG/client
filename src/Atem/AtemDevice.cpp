#include "AtemDevice.h"

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

void AtemDevice::selectInput(const QString& switcher, const QString& input)
{
    if (switcher == "pgm")
        SwitcherDevice::atemConnection->changeProgramInput(input.toInt());
    else if (switcher == "prev")
        SwitcherDevice::atemConnection->changePreviewInput(input.toInt());
}

void AtemDevice::setKeyerState(const QString& keyer, bool state)
{
    if (keyer == "0" || keyer == "1") // Downstream keyer
        SwitcherDevice::atemConnection->setDownstreamKeyOn(keyer.toInt(), state);
    else
        SwitcherDevice::atemConnection->setUpstreamKeyOn(keyer.toInt() - 2, state);
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
