#include "PanasonicDevice.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

PanasonicDevice::PanasonicDevice(QObject* parent)
    : QObject(parent)
{
}

void PanasonicDevice::selectPreset(const QString& address, int preset)
{
    preset = (preset > 0) ? preset - 1 : 0;

    QUrl request(QString("http://%1/cgi-bin/aw_ptz").arg(address));

    QUrlQuery query;
    query.addQueryItem("cmd", QString("#R%1").arg(QString(preset).sprintf("%02d", preset)));
    query.addQueryItem("res", "1");

    request.setQuery(query);

    qDebug() << QString("CameraDevice::selectPreset: %1").arg(request.toString());

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(selectPresetFinished(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(request));
}

void PanasonicDevice::selectPresetFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("CameraDevice::selectPresetFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}
