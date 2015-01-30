#include "SpyderDevice.h"

#include <QtCore/QByteArray>

#include <QtNetwork/QUdpSocket>

SpyderDevice::SpyderDevice(QObject* parent)
    : QObject(parent)
{
}

void SpyderDevice::selectPreset(int preset, const QString& address, int port)
{
    QByteArray data = QString::fromLatin1("spyder\0\0\0\0RSC %1 1", 18).arg(preset).toLatin1();

    QUdpSocket socket;
    socket.writeDatagram(data, data.size(), QHostAddress(address), port);
    socket.flush();
}
