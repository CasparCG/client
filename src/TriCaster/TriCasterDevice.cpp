#include "TriCasterDevice.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

TriCasterDevice::TriCasterDevice(const QString& address, int port, QObject* parent)
    : NTFCDevice(address, port, parent)
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

void TriCasterDevice::switchProgramInput(const QString& target)
{
    writeMessage(QString("<shortcuts><shortcut name=\"switcher_pgm_named_input\" value=\"%1\" /></shortcuts>").arg(target));
}

void TriCasterDevice::switchPreviewInput(const QString& target)
{
    writeMessage(QString("<shortcuts><shortcut name=\"switcher_prev_named_input\" value=\"%1\" /></shortcuts>").arg(target));
}

void TriCasterDevice::selectAudiomixerPreset(const QString& preset)
{
    writeMessage(QString("<shortcuts><shortcut name=\"audiomixer_select_preset\" value=\"%1\" /></shortcuts>").arg(preset));
}

void TriCasterDevice::sendNotification()
{
}
