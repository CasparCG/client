#include "SynchronizeManager.h"
#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "Events/AutoSynchronizeEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/SynchronizeEvent.h"
#include "Events/TemplateChangedEvent.h"
#include "Models/DeviceModel.h"

#include "Global.h"

#include "CasparDevice.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(SynchronizeManager, synchronizeManager)

SynchronizeManager::SynchronizeManager(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&this->synchronizeTimer, SIGNAL(timeout()), this, SLOT(synchronize()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));

    qApp->installEventFilter(this);
}

SynchronizeManager& SynchronizeManager::getInstance()
{
    return *synchronizeManager();
}

void SynchronizeManager::initialize()
{
    qApp->postEvent(qApp, new SynchronizeEvent(0));
    qApp->postEvent(qApp, new AutoSynchronizeEvent((DatabaseManager::getInstance().getConfigurationByName("AutoSynchronize").getValue() == "true") ? true : false,
                                                   DatabaseManager::getInstance().getConfigurationByName("SynchronizeInterval").getValue().toInt() * 1000));
}

void SynchronizeManager::uninitialize()
{

}

bool SynchronizeManager::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::Synchronize))
    {
        SynchronizeEvent* synchronizeEvent = dynamic_cast<SynchronizeEvent*>(event);
        QTimer::singleShot(synchronizeEvent->getDelay(), this, SLOT(synchronize()));
    }
    else if (event->type() == static_cast<QEvent::Type>(Enum::EventType::AutoSynchronize))
    {
        AutoSynchronizeEvent* autoSynchronizeEvent = dynamic_cast<AutoSynchronizeEvent*>(event);
        if (this->synchronizeTimer.interval() != autoSynchronizeEvent->getInterval())
            this->synchronizeTimer.setInterval(autoSynchronizeEvent->getInterval());

        if (this->synchronizeTimer.isActive() != autoSynchronizeEvent->getAutoSynchronize())
        {
            if (autoSynchronizeEvent->getAutoSynchronize())
                this->synchronizeTimer.start();
            else
                this->synchronizeTimer.stop();
        }
    }

    return QObject::eventFilter(target, event);
}

void SynchronizeManager::synchronize()
{
    DeviceManager::getInstance().synchronize();

    if (DeviceManager::getInstance().getConnectionCount() == 0)
        return;

    StatusbarEvent* event = new StatusbarEvent("Synchronizing library...");
    qApp->postEvent(qApp, event);

    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "Yes")
            continue;

        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getConnectionByName(model.getName());
        if (device->isConnected())
        {
            device->refreshMedia();
            device->refreshTemplate();
            device->refreshData();
        }
    }

    this->synchronizeTimer.setInterval(DatabaseManager::getInstance().getConfigurationByName("SynchronizeInterval").getValue().toInt() * 1000);
}

void SynchronizeManager::deviceRemoved()
{
    qApp->postEvent(qApp, new MediaChangedEvent());
    qApp->postEvent(qApp, new TemplateChangedEvent());
    qApp->postEvent(qApp, new DataChangedEvent());
}

void SynchronizeManager::deviceAdded(CasparDevice& device)
{
    QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(deviceConnectionStateChanged(CasparDevice&)));
    QObject::connect(&device, SIGNAL(mediaChanged(const QList<CasparMedia>&, CasparDevice&)), this, SLOT(deviceMediaChanged(const QList<CasparMedia>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(templateChanged(const QList<CasparTemplate>&, CasparDevice&)), this, SLOT(deviceTemplateChanged(const QList<CasparTemplate>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(dataChanged(const QList<CasparData>&, CasparDevice&)), this, SLOT(deviceDataChanged(const QList<CasparData>&, CasparDevice&)));
}

void SynchronizeManager::deviceConnectionStateChanged(CasparDevice& device)
{
    if (device.isConnected())
    {
        const DeviceModel& model = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
        if (model.getShadow() == "Yes")
            return;

        device.refreshMedia();
        device.refreshTemplate();
        device.refreshData();
    }
}

void SynchronizeManager::deviceMediaChanged(const QList<CasparMedia>& mediaItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryMediaByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparMedia mediaItem, mediaItems)
        {
            if (mediaItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparMedia mediaItem, mediaItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == mediaItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, mediaItem.getName(), "", mediaItem.getType()));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryMedia(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new MediaChangedEvent());
    }

    qDebug() << QString("Synchronize::deviceMediaChanged: %1 msec").arg(time.elapsed());
}

void SynchronizeManager::deviceTemplateChanged(const QList<CasparTemplate>& templateItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryTemplateByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparTemplate templateItem, templateItems)
        {
            if (templateItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparTemplate templateItem, templateItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == templateItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, templateItem.getName(), "", "TEMPLATE"));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryTemplate(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new TemplateChangedEvent());
    }

    qDebug() << QString("Synchronize::deviceTemplateChanged: %1 msec").arg(time.elapsed());
}

void SynchronizeManager::deviceDataChanged(const QList<CasparData>& dataItems, CasparDevice& device)
{
    QTime time;
    time.start();

    QList<LibraryModel> insertModels;
    QList<LibraryModel> deleteModels;
    QList<LibraryModel> libraryModels = DatabaseManager::getInstance().getLibraryDataByDeviceAddress(device.getAddress());

    // Find library items to delete.
    foreach (const LibraryModel& libraryModel, libraryModels)
    {
        bool found = false;
        foreach (CasparData dataItem, dataItems)
        {
            if (dataItem.getName() == libraryModel.getName())
                found = true;
        }

        if (!found)
            deleteModels.push_back(libraryModel);
    }

    // Find library items to insert.
    foreach (CasparData dataItem, dataItems)
    {
        bool found = false;
        foreach (const LibraryModel& libraryModel, libraryModels)
        {
            if (libraryModel.getName() == dataItem.getName())
                found = true;
        }

        if (!found)
            insertModels.push_back(LibraryModel(0, dataItem.getName(), "", "DATA"));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryData(device.getAddress(), deleteModels, insertModels);
        qApp->postEvent(qApp, new DataChangedEvent());
    }

    qDebug() << QString("Synchronize::deviceDataChanged: %1 msec").arg(time.elapsed());
}
