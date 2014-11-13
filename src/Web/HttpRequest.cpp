#include "HttpRequest.h"

#include <QtCore/QDebug>
#include <QtCore/QPair>

HttpRequest::HttpRequest(QObject* parent)
    : QObject(parent)
{
}

void HttpRequest::sendGet(const QString& url, const QUrl& data)
{
    QUrl request(url);

    typedef QPair<QString, QString> QueryPair; // Workaround - foreach does not support templates.
    foreach (QueryPair queryItem, data.queryItems())
        request.addQueryItem(queryItem.first, queryItem.second);

    qDebug() << QString("HttpRequest::sendGet: %1").arg(request.toString());

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendGetFinished(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(request));
}

void HttpRequest::sendGetFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("HttpRequest::sendGetFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}

void HttpRequest::sendPost(const QString& url, const QUrl& data)
{
    qDebug() << QString("HttpRequest::sendPost: %1, %2").arg(url).arg(QString(data.encodedQuery()));

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendPostFinished(QNetworkReply*)));
    this->networkManager->post(QNetworkRequest(QUrl(url)), data.encodedQuery());
}

void HttpRequest::sendPostFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("HttpRequest::sendPostFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}
