#include "OscSubscription.h"
#include "OscDeviceManager.h"
#include "OscWebSocketManager.h"

#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

OscSubscription::OscSubscription(const QString& path, QObject *parent)
    : QObject(parent),
      path(path)
{  
    QObject::connect(OscDeviceManager::getInstance().getOscMonitorListener().data(), SIGNAL(messageReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(messageReceived(const QString&, const QList<QVariant>&)));
    QObject::connect(OscDeviceManager::getInstance().getOscControlListener().data(), SIGNAL(messageReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(messageReceived(const QString&, const QList<QVariant>&)));
    QObject::connect(OscWebSocketManager::getInstance().getOscWebSocketListener().data(), SIGNAL(messageReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(messageReceived(const QString&, const QList<QVariant>&)));
}

void OscSubscription::messageReceived(const QString& pattern, const QList<QVariant>& arguments)
{
    if (pattern.endsWith(this->path))
    {
        //qDebug("Found a subscriber: %s:%s", qPrintable(this->parent()), qPrintable(this->path));

        emit subscriptionReceived(this->path, arguments);
    }
}
