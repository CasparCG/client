#include "ThumbnailWorker.h"
#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "Events/MediaChangedEvent.h"
#include "Events/StatusbarEvent.h"

#include "Global.h"

#include "CasparDevice.h"

#include <QtCore/QTimer>

#include <QtGui/QApplication>

ThumbnailWorker::ThumbnailWorker(const QList<ThumbnailModel>& thumbnailModels, QObject* parent)
    : QObject(parent),
      thumbnailModels(thumbnailModels)
{
    QObject::connect(&this->thumbnailTimer, SIGNAL(timeout()), this, SLOT(process()));
}

void ThumbnailWorker::start()
{
    this->thumbnailTimer.start();
    this->thumbnailTimer.setInterval(2000); // Settings?
}

void ThumbnailWorker::process()
{
    if (this->thumbnailModels.count() == 0 || DeviceManager::getInstance().getConnectionCount() == 0)
    {
        this->thumbnailTimer.stop();
        qApp->postEvent(qApp, new MediaChangedEvent());

        return;
    }

    qDebug() << this->thumbnailModels.count();

    this->currentId = this->thumbnailModels.at(0).getId();
    this->currentName = this->thumbnailModels.at(0).getName();
    this->currentAddress = this->thumbnailModels.at(0).getAddress();
    this->currentTimestamp= this->thumbnailModels.at(0).getTimestamp();

    qApp->postEvent(qApp, new StatusbarEvent(QString("Retrieving thumbnail %1...").arg(this->currentName)));
    qDebug() << QString("Processing thumbnail %1, Id %2").arg(this->currentName).arg(this->currentId);

    const DeviceModel& model = DeviceManager::getInstance().getDeviceModelByAddress(this->currentAddress);
    if (model.getShadow() == "Yes")
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(model.getName());
    if (device->isConnected())
    {
        QObject::connect(device.data(), SIGNAL(thumbnailRetrieveChanged(const QString&, CasparDevice&)), this, SLOT(deviceThumbnailRetrieveChanged(const QString&, CasparDevice&)));
        device->retrieveThumbnail(this->currentName);
    }

    this->thumbnailModels.removeAt(0);
}

void ThumbnailWorker::deviceThumbnailRetrieveChanged(const QString& data, CasparDevice& device)
{
    device.disconnect(); // Disconnect events.

    DatabaseManager::getInstance().updateThumbnail(ThumbnailModel(0, data, this->currentTimestamp, this->currentName, this->currentAddress));
}
