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
#include "Models/Atem/AtemDeviceModel.h"
#include "Models/Atem/AtemAudioInputModel.h"
#include "Models/Atem/AtemAudioInputStateModel.h"
#include "Models/Atem/AtemSwitcherModel.h"
#include "Models/Atem/AtemStepModel.h"
#include "Models/Atem/AtemMixerStepModel.h"
#include "Models/Atem/AtemAutoTransitionModel.h"
#include "Models/Atem/AtemKeyerModel.h"
#include "Models/Atem/AtemVideoFormatModel.h"
#include "Models/TriCaster/TriCasterProductModel.h"
#include "Models/TriCaster/TriCasterInputModel.h"
#include "Models/TriCaster/TriCasterStepModel.h"
#include "Models/TriCaster/TriCasterAutoSpeedModel.h"
#include "Models/TriCaster/TriCasterAutoTransitionModel.h"
#include "Models/TriCaster/TriCasterPresetModel.h"
#include "Models/TriCaster/TriCasterSourceModel.h"
#include "Models/TriCaster/TriCasterSwitcherModel.h"
#include "Models/TriCaster/TriCasterDeviceModel.h"
#include "Models/TriCaster/TriCasterNetworkTargetModel.h"

#include <QtCore/QMutex>
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

        QList<AtemStepModel> getAtemStep();
        QList<AtemMixerStepModel> getAtemMixerStep();
        QList<AtemAudioInputStateModel> getAtemAudioInputState();
        QList<AtemKeyerModel> getAtemKeyer();
        QList<AtemSwitcherModel> getAtemSwitcher();
        QList<AtemVideoFormatModel> getAtemVideoFormat();
        QList<AtemAutoTransitionModel> getAtemAutoTransition();
        QList<AtemDeviceModel> getAtemDevice();
        AtemDeviceModel getAtemDeviceByName(const QString& name);
        AtemDeviceModel getAtemDeviceByAddress(const QString& address);
        void insertAtemDevice(const AtemDeviceModel& model);
        void updateAtemDevice(const AtemDeviceModel& model);
        void deleteAtemDevice(int id);

        QList<TriCasterProductModel> getTriCasterProduct();
        QList<TriCasterInputModel> getTriCasterInput();
        QList<TriCasterStepModel> getTriCasterStep();
        QList<TriCasterAutoSpeedModel> getTriCasterAutoSpeed();
        QList<TriCasterAutoTransitionModel> getTriCasterAutoTransition();
        QList<TriCasterPresetModel> getTriCasterPreset();
        QList<TriCasterSourceModel> getTriCasterSource();
        QList<TriCasterSwitcherModel> getTriCasterSwitcher();
        QList<TriCasterDeviceModel> getTriCasterDevice();
        QList<TriCasterNetworkTargetModel> getTriCasterNetworkTarget();
        TriCasterDeviceModel getTriCasterDeviceByName(const QString& name);
        TriCasterDeviceModel getTriCasterDeviceByAddress(const QString& address);
        void insertTriCasterDevice(const TriCasterDeviceModel& model);
        void updateTriCasterDevice(const TriCasterDeviceModel& model);
        void deleteTriCasterDevice(int id);

        QList<GpiPortModel> getGpiPorts();
        QList<GpoPortModel> getGpoPorts();
        void updateGpiPort(const GpiPortModel& model);
        void updateGpoPort(const GpoPortModel& model);

        QList<TypeModel> getType();
        TypeModel getTypeByValue(const QString& value);

        QList<DeviceModel> getDevice();
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
        QMutex mutex;

        void createDatabase();
        void upgradeDatabase();
};
