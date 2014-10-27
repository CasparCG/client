#pragma once

#include "Shared.h"

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class WEB_EXPORT HttpRequest : public QObject
{
    Q_OBJECT

    public:
        explicit HttpRequest(QObject* parent = 0);

        void sendGet(const QString& url, const QByteArray& data);
        void sendPost(const QString& url, const QByteArray& data);

    private:
        QNetworkAccessManager* networkManager;

        Q_SLOT void sendGetFinished(QNetworkReply*);
        Q_SLOT void sendPostFinished(QNetworkReply*);
};
