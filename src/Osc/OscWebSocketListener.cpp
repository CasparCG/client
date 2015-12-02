#include "OscWebSocketListener.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

OscWebSocketListener::OscWebSocketListener(QObject* parent)
    : QObject(parent)
{
}

OscWebSocketListener::~OscWebSocketListener()
{
    this->server->close();
    qDeleteAll(this->sockets.begin(), this->sockets.end());
}

void OscWebSocketListener::start(int port)
{
    this->server = new QWebSocketServer("", QWebSocketServer::NonSecureMode, this);
    if (this->server->listen(QHostAddress::Any, port))
    {
        qDebug("Listening for incoming OSC messages over WebSocket on port %d", port);

        QObject::connect(this->server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
    else
    {
        qWarning("Unable to listen on port %d", port);
    }
}

void OscWebSocketListener::newConnection()
{
    QWebSocket* socket = this->server->nextPendingConnection();

    qDebug("Accepted connection from %s", qPrintable(socket->peerAddress().toString()));

    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));

    this->sockets.append(socket);
}

/*
 * The protocol for OSC over WebSockets is a simple JSON description.
 *
 * {
 *      "path" : "/valid/path",
 *      "args" : [ arg1, arg2, ... ]
 * }
 *
 * For a complete list of valid OSC paths, look in Global.h. First argument
 * must always be a number greater than zero to be executed.
 */
void OscWebSocketListener::textMessageReceived(const QString& message)
{
    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject object = document.object();

    QString path = object.value("path").toString();

    QList<QVariant> arguments;
    QJsonArray array = object.value("args").toArray();
    for (int i = 0; i < array.count(); i++)
    {
        if (array[i].isBool())
            arguments.append(array[i].toBool());
        else if (array[i].isDouble())
            arguments.append(array[i].toDouble());
        else if (array[i].isString())
            arguments.append(array[i].toString());
    }

    if (!path.isEmpty() && arguments.count() > 0)
    {
        qDebug("Received OSC message over WebSocket: %s", qPrintable(path));

        emit messageReceived(path, arguments);
    }
}

void OscWebSocketListener::disconnected()
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(QObject::sender());

    qDebug("Client %s was disconnected", qPrintable(socket->peerAddress().toString()));

    if (socket != nullptr)
    {
        this->sockets.removeAll(socket);
        socket->deleteLater();
    }
}
