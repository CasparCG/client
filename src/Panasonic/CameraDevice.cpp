#include "CameraDevice.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>

CameraDevice::CameraDevice(QObject* parent)
    : QObject(parent)
{
}

void CameraDevice::selectPreset(const QString& address, int preset)
{
    preset = (preset > 0) ? preset - 1 : 0;

    QUrl request(QString("http://%1/cgi-bin/aw_ptz").arg(address));
    request.addQueryItem("cmd", QString("#R%1").arg(QString(preset).sprintf("%02d", preset)));
    request.addQueryItem("res", "1");

    qDebug() << QString("CameraDevice::selectPreset: %1").arg(request.toString());

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(selectPresetFinished(QNetworkReply*)));
    this->networkManager->get(QNetworkRequest(request));
}

void CameraDevice::selectPresetFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("CameraDevice::selectPresetFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}
