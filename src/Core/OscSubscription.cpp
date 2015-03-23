#include "OscSubscription.h"
#include "OscDeviceManager.h"

#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

OscSubscription::OscSubscription(const QString& path, QObject *parent)
    : QObject(parent),
      path(path)
{  
    QObject::connect(OscDeviceManager::getInstance().getOscListener().data(), SIGNAL(messageReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(messageReceived(const QString&, const QList<QVariant>&)));
}

void OscSubscription::messageReceived(const QString& pattern, const QList<QVariant>& arguments)
{
    if (pattern.endsWith(this->path))
    {
        //qDebug() << "Found a subscriber: " << this->parent() << ":" << this->path;

        emit subscriptionReceived(this->path, arguments);
    }
}
