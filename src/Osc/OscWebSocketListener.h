#pragma once

#include "Shared.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class QWebSocket;
class QWebSocketServer;

class OSC_EXPORT OscWebSocketListener : public QObject
{
    Q_OBJECT

    public:
        explicit OscWebSocketListener(QObject* parent = 0);
        ~OscWebSocketListener();

        void start(int port);

        Q_SIGNAL void messageReceived(const QString&, const QList<QVariant>&);

    private:
        QList<QWebSocket*> sockets;
        QWebSocketServer* server = nullptr;

        Q_SLOT void disconnected();
        Q_SLOT void newConnection();
        Q_SLOT void textMessageReceived(const QString& message);
};
