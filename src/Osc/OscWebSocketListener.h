#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QMutex>

class OSC_EXPORT OscWebSocketListener : public QObject
{
    Q_OBJECT

    public:
        explicit OscWebSocketListener(const QString& address, int port, QObject* parent = 0);
        ~OscWebSocketListener();

        void start();

        Q_SIGNAL void messageReceived(const QString&, const QList<QVariant>&);

    private:
        QMutex eventsMutex;
        QMap<QString, QList<QVariant>> events;

        QWebSocketServer* socket = nullptr;

        Q_SLOT void sendEventBatch();
        Q_SLOT void textMessageReceived(const QString& message);
};
