#include "OscMessage.h"
#include "DeviceManager.h"

#include <QtCore/QSharedPointer>

OscMessage::OscMessage(const QString& predicate, QObject *parent)
    : QObject(parent),
      predicate(predicate)
{  
    QObject::connect(DeviceManager::getInstance().getOscListener().data(), SIGNAL(oscMessageReceived(const QString&, const QString&, const QList<QVariant>&)),
                     this, SLOT(oscMessageReceived(const QString&, const QString&, const QList<QVariant>&)));
}

void OscMessage::oscMessageReceived(const QString& address, const QString& pattern, const QList<QVariant>& arguments)
{
    if (predicate.exactMatch(QString("/%1%2").arg(address).arg(pattern)))
        emit messageReceived(predicate.pattern(), arguments);
}
