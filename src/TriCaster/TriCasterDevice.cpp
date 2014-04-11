#include "TriCasterDevice.h"

#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

TriCasterDevice::TriCasterDevice(const QString& address, int port, QObject* parent)
    : NtfcDevice(address, port, parent)
{
}

void TriCasterDevice::triggerAuto(const QString& target, const QString& speed, const QString& transition)
{
    if (!speed.isEmpty())
        writeMessage(QString("<shortcuts><shortcut name=\"%1_%2\" value=\"true\" /></shortcuts>").arg(target).arg(speed));

    if (!transition.isEmpty())
        writeMessage(QString("<shortcuts><shortcut name=\"%1_select_index\" value=\"%2\" /></shortcuts>").arg(target).arg(transition));

    writeMessage(QString("<shortcuts><shortcut name=\"%1_auto\" /></shortcuts>").arg(target));
}

void TriCasterDevice::triggerTake(const QString& target)
{
    writeMessage(QString("<shortcuts><shortcut name=\"%1_take\" /></shortcuts>").arg(target));
}

void TriCasterDevice::selectInput(const QString& switcher, const QString& input, const QString& product)
{
    if (product == "TriCaster 850")
        writeMessage(QString("<shortcuts><shortcut name=\"switcher_%1_named_input\" value=\"%2\" /></shortcuts>").arg(switcher).arg(input));
    else
    {
        if (switcher == "pgm")
            writeMessage(QString("<shortcuts><shortcut name=\"main_a_row_named_input\" value=\"%1\" /></shortcuts>").arg(input));
        else if (switcher == "prev")
            writeMessage(QString("<shortcuts><shortcut name=\"main_b_row_named_input\" value=\"%1\" /></shortcuts>").arg(input));
    }
}

void TriCasterDevice::selectNetworkSource(const QString& target, const QString& source)
{
    writeMessage(QString("<shortcuts><shortcut name=\"%1_source\" value=\"%2\" /></shortcuts>").arg(target).arg(source));
}

void TriCasterDevice::playMacro(const QString& name)
{
    writeMessage(QString("<shortcuts><shortcut name=\"play_macro_byname\" value=\"%1\" /></shortcuts>").arg(name));
}

void TriCasterDevice::selectPreset(const QString& source, const QString& preset)
{
    writeMessage(QString("<shortcuts><shortcut name=\"%1_select_preset\" value=\"%2\" /></shortcuts>").arg(source).arg(preset));
}

void TriCasterDevice::sendNotification()
{
    switch (NtfcDevice::command)
    {
        case NtfcDevice::CONNECTIONSTATE:
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
