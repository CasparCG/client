#include "CameraDevice.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>

CameraDevice::CameraDevice(QObject* parent)
    : QObject(parent)
{
}

void CameraDevice::selectPreset(const QString& address, const QString& preset)
{
    QString url = QString("http://%1/cgi-bin/aw_ptz").arg(address);
    QString data = QString("#R%1&res=1").arg(preset);

    qDebug() << QString("CameraDevice::selectPreset: %1?%2").arg(url).arg(data);

    this->networkManager = new QNetworkAccessManager(this);
    QObject::connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(selectPresetFinished(QNetworkReply*)));
    this->networkManager->post(QNetworkRequest(QUrl(url)), data.toUtf8());
}

void CameraDevice::selectPresetFinished(QNetworkReply* reply)
{
    QString data = QString::fromUtf8(reply->readAll());

    qDebug() << QString("CameraDevice::selectPresetFinished: %1").arg(data);

    reply->deleteLater();
    this->networkManager->deleteLater();
}
