#include "HttpRequest.h"

#include <QtCore/QDebug>

HttpRequest::HttpRequest(QObject* parent)
    : QObject(parent)
{
}

void HttpRequest::sendGet(const QString& url, const QByteArray& data)
{
    QString request = QString("%1?%2").arg(url).arg(QString(data));

    qDebug() << QString("HttpRequest::sendGet: %1").arg(request);

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendGetFinished(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(QUrl(request)));
}

void HttpRequest::sendGetFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("HttpRequest::sendGetFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}

void HttpRequest::sendPost(const QString& url, const QByteArray& data)
{
    qDebug() << QString("HttpRequest::sendPost: %1, %2").arg(url).arg(QString(data));

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendPostFinished(QNetworkReply*)));
    this->networkManager->post(QNetworkRequest(QUrl(url)), data);
}

void HttpRequest::sendPostFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("HttpRequest::sendPostFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}
