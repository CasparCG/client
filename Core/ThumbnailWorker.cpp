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
    connect(&this->thumbnailTimer, SIGNAL(timeout()), this, SLOT(process()));
}

void ThumbnailWorker::start()
{
    this->thumbnailTimer.setInterval(2000); // Settings?
    this->thumbnailTimer.start();
}

void ThumbnailWorker::process()
{
    if (this->thumbnailModels.count() == 0)
    {
        this->thumbnailTimer.stop();
        qApp->postEvent(qApp, new MediaChangedEvent());

        return;
    }

    this->currentName = this->thumbnailModels.at(0).getName();
    this->currentAddress = this->thumbnailModels.at(0).getAddress();
    this->currentTimestamp= this->thumbnailModels.at(0).getTimestamp();
    this->currentSize= this->thumbnailModels.at(0).getSize();

    const DeviceModel& model = DeviceManager::getInstance().getDeviceModelByAddress(this->currentAddress);
    if (model.getShadow() == "Yes")
        return;

    const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(model.getName());
    if (!device->isConnected())
    {
        this->thumbnailTimer.stop();
        qApp->postEvent(qApp, new MediaChangedEvent());

        return;
    }

    qApp->postEvent(qApp, new StatusbarEvent(QString("Retrieving thumbnail %1...").arg(this->currentName)));
    qDebug() << QString("ThumbnailWorker::process: Retrieving thumbnail %1").arg(this->currentName);

    connect(device.data(), SIGNAL(thumbnailRetrieveChanged(const QString&, CasparDevice&)), this, SLOT(thumbnailRetrieveChanged(const QString&, CasparDevice&)));
    device->retrieveThumbnail(this->currentName);

    this->thumbnailModels.removeAt(0);
}

void ThumbnailWorker::thumbnailRetrieveChanged(const QString& data, CasparDevice& device)
{
    QObject::disconnect(&device, SIGNAL(thumbnailRetrieveChanged(const QString&, CasparDevice&)), this, SLOT(thumbnailRetrieveChanged(const QString&, CasparDevice&)));
    DatabaseManager::getInstance().updateThumbnail(ThumbnailModel(0, data, this->currentTimestamp, this->currentSize, this->currentName, this->currentAddress));
}
