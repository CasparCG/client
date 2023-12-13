#pragma once

#include "Shared.h"
#include "Models/BlendModeModel.h"
#include "Models/ConfigurationModel.h"
#include "Models/ChromaModel.h"
#include "Models/DeviceModel.h"
#include "Models/DirectionModel.h"
#include "Models/FormatModel.h"
#include "Models/LibraryModel.h"
#include "Models/GpiModel.h"
#include "Models/GpiPortModel.h"
#include "Models/GpoPortModel.h"
#include "Models/RundownModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"
#include "Models/TypeModel.h"
#include "Models/ThumbnailModel.h"
#include "Models/FormatModel.h"
#include "Models/PresetModel.h"
#include "Models/OscOutputModel.h"

#include <QtCore/QRecursiveMutex>
#include <QtCore/QObject>

class CORE_EXPORT DatabaseManager
{
    public:
        explicit DatabaseManager();

        static DatabaseManager& getInstance();

        void initialize();
        void uninitialize() {}

        ConfigurationModel getConfigurationByName(const QString& name);
        void updateConfiguration(const ConfigurationModel& model);

        QList<FormatModel> getFormat();
        FormatModel getFormat(const QString& name);

        QList<QString> getOpenRecent();
        void insertOpenRecent(const QString& path);
        void deleteOpenRecent();

        QList<PresetModel> getPreset();
        PresetModel getPreset(const QString& name);
        QList<PresetModel> getPresetByFilter(const QString& filter);
        void insertPreset(const PresetModel& model);
        void deletePreset(int id);

        QList<BlendModeModel> getBlendMode();
        QList<ChromaModel> getChroma();
        QList<DirectionModel> getDirection();
        QList<TransitionModel> getTransition();
        QList<TweenModel> getTween();

        QList<OscOutputModel> getOscOutput();
        void insertOscOutput(const OscOutputModel& model);
        OscOutputModel getOscOutputByName(const QString& name);
        OscOutputModel getOscOutputByAddress(const QString& address);
        void updateOscOutput(const OscOutputModel& model);
        void deleteOscOutput(int id);

        QList<GpiPortModel> getGpiPorts();
        QList<GpoPortModel> getGpoPorts();
        void updateGpiPort(const GpiPortModel& model);
        void updateGpoPort(const GpoPortModel& model);

        QList<TypeModel> getType();
        TypeModel getTypeByValue(const QString& value);

        QList<DeviceModel> getDevice();
        DeviceModel getDeviceById(int deviceId);
        DeviceModel getDeviceByName(const QString& name);
        DeviceModel getDeviceByAddress(const QString& address);
        void insertDevice(const DeviceModel& model);
        void updateDevice(const DeviceModel& model);
        void updateDeviceVersion(const DeviceModel& model);
        void updateDeviceChannels(const DeviceModel& model);
        void updateDeviceChannelFormats(const DeviceModel& model);
        void deleteDevice(int id);

        QList<LibraryModel> getLibraryMedia();
        QList<LibraryModel> getLibraryTemplate();
        QList<LibraryModel> getLibraryData();
        QList<LibraryModel> getLibraryMediaByFilter(const QString& filter, QList<QString> devices);
        QList<LibraryModel> getLibraryTemplateByFilter(const QString& filter, QList<QString> devices);
        QList<LibraryModel> getLibraryDataByFilter(const QString& filter, QList<QString> devices);
        QList<LibraryModel> getLibraryByDeviceId(int deviceId);
        QList<LibraryModel> getLibraryByDeviceIdAndFilter(int deviceId, const QString& filter);
        QList<LibraryModel> getLibraryMediaByDeviceAddress(const QString& address);
        QList<LibraryModel> getLibraryTemplateByDeviceAddress(const QString& address);
        QList<LibraryModel> getLibraryDataByDeviceAddress(const QString& address);
        QList<LibraryModel> getLibraryByNameAndDeviceId(const QString& name, int deviceId);
        void updateLibraryMedia(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void updateLibraryTemplate(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void updateLibraryData(const QString& address, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void deleteLibrary(int deviceId);

        QList<ThumbnailModel> getThumbnailByDeviceAddress(const QString& address);
        ThumbnailModel getThumbnailByNameAndDeviceName(const QString& name, const QString& deviceName);
        void updateThumbnail(const ThumbnailModel& model);
        void deleteThumbnails();

    private:
        QRecursiveMutex mutex;

        void createDatabase();
        void upgradeDatabase();
};
