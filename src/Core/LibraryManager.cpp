#include "LibraryManager.h"
#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"
#include "AtemDeviceManager.h"
#include "TriCasterDeviceManager.h"
#include "Events/StatusbarEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Models/DeviceModel.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtCore/QStringList>

#include <QtGui/QApplication>

Q_GLOBAL_STATIC(LibraryManager, libraryManager)

LibraryManager::LibraryManager(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&this->refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceRemoved()), this, SLOT(deviceRemoved()));
    QObject::connect(&DeviceManager::getInstance(), SIGNAL(deviceAdded(CasparDevice&)), this, SLOT(deviceAdded(CasparDevice&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(autoRefreshLibrary(const AutoRefreshLibraryEvent&)), this, SLOT(autoRefreshLibrary(const AutoRefreshLibraryEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(refreshLibrary(const RefreshLibraryEvent&)), this, SLOT(refreshLibrary(const RefreshLibraryEvent&)));
}

LibraryManager& LibraryManager::getInstance()
{
    return *libraryManager();
}

void LibraryManager::initialize()
{
    EventManager::getInstance().fireRefreshLibraryEvent(RefreshLibraryEvent());
    EventManager::getInstance().fireAutoRefreshLibraryEvent(AutoRefreshLibraryEvent((DatabaseManager::getInstance().getConfigurationByName("AutoRefreshLibrary").getValue() == "true") ? true : false,
                                                                                    DatabaseManager::getInstance().getConfigurationByName("RefreshLibraryInterval").getValue().toInt() * 1000));
}

void LibraryManager::uninitialize()
{
}

void LibraryManager::refreshLibrary(const RefreshLibraryEvent& event)
{
    QTimer::singleShot(event.getDelay(), this, SLOT(refresh()));
}

void LibraryManager::autoRefreshLibrary(const AutoRefreshLibraryEvent& event)
{
    if (this->refreshTimer.interval() != event.getInterval())
        this->refreshTimer.setInterval(event.getInterval());

    if (this->refreshTimer.isActive() != event.getAutoRefresh())
    {
        if (event.getAutoRefresh())
            this->refreshTimer.start();
        else
            this->refreshTimer.stop();
    }
}

void LibraryManager::refresh()
{
    // Clear out all thumbnail workers.
    this->thumbnailWorkers.clear();

    DeviceManager::getInstance().refresh();
    AtemDeviceManager::getInstance().refresh();
    TriCasterDeviceManager::getInstance().refresh();

    if (DeviceManager::getInstance().getDeviceCount() == 0)
        return;

    EventManager::getInstance().fireStatusbarEvent(StatusbarEvent("Refreshing library..."));
    qDebug() << QString("LibraryManager::refresh: Refreshing library...");

    // Only refresh library for all devices.
    foreach (const DeviceModel& model, DeviceManager::getInstance().getDeviceModels())
    {
        if (model.getShadow() == "Yes")
            continue;

        const QSharedPointer<CasparDevice> device = DeviceManager::getInstance().getDeviceByName(model.getName());
        if (device != NULL && device->isConnected())
        {
            device->refreshServerVersion();
            device->refreshChannels();
            device->refreshMedia();
            device->refreshTemplate();
            device->refreshData();
            device->refreshThumbnail();
        }
    }

    this->refreshTimer.setInterval(DatabaseManager::getInstance().getConfigurationByName("RefreshLibraryInterval").getValue().toInt() * 1000);
}

void LibraryManager::deviceRemoved()
{
    EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
    EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
    EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
}

void LibraryManager::deviceAdded(CasparDevice& device)
{
    QObject::connect(&device, SIGNAL(versionChanged(const QString&, CasparDevice&)), this, SLOT(versionChanged(const QString&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(infoChanged(const QList<QString>&, CasparDevice&)), this, SLOT(infoChanged(const QList<QString>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(connectionStateChanged(CasparDevice&)), this, SLOT(connectionStateChanged(CasparDevice&)));
    QObject::connect(&device, SIGNAL(mediaChanged(const QList<CasparMedia>&, CasparDevice&)), this, SLOT(mediaChanged(const QList<CasparMedia>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(templateChanged(const QList<CasparTemplate>&, CasparDevice&)), this, SLOT(templateChanged(const QList<CasparTemplate>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(dataChanged(const QList<CasparData>&, CasparDevice&)), this, SLOT(dataChanged(const QList<CasparData>&, CasparDevice&)));
    QObject::connect(&device, SIGNAL(thumbnailChanged(const QList<CasparThumbnail>&, CasparDevice&)), this, SLOT(thumbnailChanged(const QList<CasparThumbnail>&, CasparDevice&)));
}

void LibraryManager::versionChanged(const QString& version, CasparDevice& device)
{
    DatabaseManager::getInstance().updateDeviceVersion(DeviceModel(0, "", device.getAddress(), 0, "", "", "", version, "", 0, ""));
}

void LibraryManager::infoChanged(const QList<QString>& info, CasparDevice& device)
{
    QStringList channelFormats;
    foreach (const QString& channelInfo, info)
        channelFormats.push_back(channelInfo.split(" ")[1]);

    DatabaseManager::getInstance().updateDeviceChannels(DeviceModel(0, "", device.getAddress(), 0, "", "", "", "", "", info.count(), ""));
    DatabaseManager::getInstance().updateDeviceChannelFormats(DeviceModel(0, "", device.getAddress(), 0, "", "", "", "", "", 0, channelFormats.join(",")));
}

void LibraryManager::connectionStateChanged(CasparDevice& device)
{
    // Only refresh library for current device.
    if (device.isConnected())
    {
        const DeviceModel& model = DeviceManager::getInstance().getDeviceModelByAddress(device.getAddress());
        if (model.getShadow() == "Yes")
            return;

        device.refreshServerVersion();
        device.refreshChannels();
        device.refreshMedia();
        device.refreshTemplate();
        device.refreshData();
        device.refreshThumbnail();
    }
}

void LibraryManager::mediaChanged(const QList<CasparMedia>& mediaItems, CasparDevice& device)
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
            insertModels.push_back(LibraryModel(0, mediaItem.getName(), mediaItem.getName(), "", mediaItem.getType(), 0, mediaItem.getTimecode()));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryMedia(device.getAddress(), deleteModels, insertModels);
        EventManager::getInstance().fireMediaChangedEvent(MediaChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceMediaChanged: %1 msec").arg(time.elapsed());
}

void LibraryManager::templateChanged(const QList<CasparTemplate>& templateItems, CasparDevice& device)
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
            insertModels.push_back(LibraryModel(0, templateItem.getName(), templateItem.getName(), "", "TEMPLATE", 0, ""));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryTemplate(device.getAddress(), deleteModels, insertModels);
        EventManager::getInstance().fireTemplateChangedEvent(TemplateChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceTemplateChanged: %1 msec").arg(time.elapsed());
}

void LibraryManager::dataChanged(const QList<CasparData>& dataItems, CasparDevice& device)
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
            insertModels.push_back(LibraryModel(0, dataItem.getName(), dataItem.getName(), "", "DATA", 0, ""));
    }

    if (deleteModels.count() > 0 || insertModels.count() > 0)
    {
        DatabaseManager::getInstance().updateLibraryData(device.getAddress(), deleteModels, insertModels);
        EventManager::getInstance().fireDataChangedEvent(DataChangedEvent());
    }

    qDebug() << QString("LibraryManager::deviceDataChanged: %1 msec").arg(time.elapsed());
}

void LibraryManager::thumbnailChanged(const QList<CasparThumbnail>& thumbnailItems, CasparDevice& device)
{
    QList<ThumbnailModel> processModels;
    QList<ThumbnailModel> thumbnailModels = DatabaseManager::getInstance().getThumbnailByDeviceAddress(device.getAddress());

    // Find thumbnail items to process.
    foreach (CasparThumbnail thumbnailItem, thumbnailItems)
    {
        bool found = false;
        foreach (const ThumbnailModel& thumbnailModel, thumbnailModels)
        {
            if (thumbnailModel.getName() == thumbnailItem.getName() &&
                thumbnailModel.getTimestamp() == thumbnailItem.getTimestamp() &&
                thumbnailModel.getSize() == thumbnailItem.getSize())
                found = true;
        }

        if (!found)
            processModels.push_back(ThumbnailModel(0, "", thumbnailItem.getTimestamp(), thumbnailItem.getSize(),
                                                   thumbnailItem.getName(), device.getAddress()));
    }

    QSharedPointer<ThumbnailWorker> thumbnailWorker(new ThumbnailWorker(processModels));
    thumbnailWorker->start();

    this->thumbnailWorkers.push_back(thumbnailWorker);
}
