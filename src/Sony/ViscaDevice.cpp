#include "ViscaDevice.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include <QtNetwork/QUdpSocket>

ViscaDevice::ViscaDevice(QObject* parent)
    : QObject(parent), preset(""), address("")
{
}

void ViscaDevice::selectPreset(const QString& address, const QString& preset)
{
    this->preset = preset;
    this->address = address;

    QTimer::singleShot(0, this, SLOT(sendClearSequence()));
    QTimer::singleShot(80, this, SLOT(sendRecallPreset())); // 1 V-cycle: PAL/50i = 20msec, NTSC/5994i = 16.7. Max 4 V-cycle for ACK/Compleation.
}

void ViscaDevice::sendClearSequence()
{
    QByteArray data;
    data.resize(9);
    data[0] = VISCA_COMMAND_CONTROL;    // Command type
    data[1] = 0x00;                     // Command type
    data[2] = 0x00;                     // Payload length
    data[3] = 0x01;                     // Payload length
    data[4] = 0x00;                     // Sequence number is ignored
    data[5] = 0x00;                     // Sequence number is ignored
    data[6] = 0x00;                     // Sequence number is ignored
    data[7] = 0x00;                     // Sequence number is ignored
    data[8] = VISCA_RESET_SEQNR;        // Command packet

    QUdpSocket socket;
    socket.writeDatagram(data, data.size(), QHostAddress(this->address), 52381);
    socket.flush();
}

void ViscaDevice::sendRecallPreset()
{
    // Command packet: 81 01 04 3F 02 0P FF, where P = Preset
    QByteArray data;
    data.resize(15);
    data[0] = VISCA_COMMAND;            // Command type
    data[1] = 0x00;                     // Command type
    data[2] = 0x00;                     // Payload length
    data[3] = 0x07;                     // Payload length
    data[4] = 0x00;                     // Sequence number
    data[5] = 0x00;                     // Sequence number
    data[6] = 0x00;                     // Sequence number
    data[7] = 0x01;                     // Sequence number
    data[8] = 0x81;                     // Command packet
    data[9] = 0x01;                     // Command packet
    data[10] = 0x04;                    // Command packet
    data[11] = 0x3F;                    // Command packet
    data[12] = 0x02;                    // Command packet
    data[13] = this->preset.toInt();    // Command packet (Our preset)
    data[14] = VISCA_TERMINATOR;

    QUdpSocket socket;
    socket.writeDatagram(data, data.size(), QHostAddress(this->address), 52381);
    socket.flush();
}
