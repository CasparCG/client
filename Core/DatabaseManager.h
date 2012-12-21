#pragma once

#include "Shared.h"
#include "Models/BlendModeModel.h"
#include "Models/ConfigurationModel.h"
#include "Models/DeviceModel.h"
#include "Models/DirectionModel.h"
#include "Models/FormatModel.h"
#include "Models/LibraryModel.h"
#include "Models/GpiDeviceModel.h"
#include "Models/GpiModel.h"
#include "Models/GpiPortModel.h"
#include "Models/GpoPortModel.h"
#include "Models/RundownModel.h"
#include "Models/TransitionModel.h"
#include "Models/TweenModel.h"
#include "Models/TypeModel.h"

#include <QtCore/QMutex>
#include <QtCore/QObject>

class CORE_EXPORT DatabaseManager
{
    public:
        explicit DatabaseManager();

        static DatabaseManager& getInstance();

        void initialize();

        void updateConfiguration(const ConfigurationModel& model);
        ConfigurationModel getConfigurationByName(const QString& name);

        QList<BlendModeModel> getBlendMode();
        QList<DirectionModel> getDirection();
        QList<FormatModel> getFormat();
        QList<TransitionModel> getTransition();
        QList<TweenModel> getTween();

        GpiDeviceModel getGpiDevice();
        void updateGpiDevice(const GpiDeviceModel& model);
        QList<GpiPortModel> getGpiPorts();
        void updateGpiPort(const GpiPortModel& model);
        QList<GpoPortModel> getGpoPorts();
        void updateGpoPort(const GpoPortModel& model);

        QList<TypeModel> getType();
        TypeModel getTypeByValue(const QString& value);

        QList<RundownModel> getRundown();
        QList<RundownModel> getRundownByFilter(const QString& filter);
        RundownModel getRundownById(int id);
        void deleteRundown(int id);

        QList<DeviceModel> getDevice();
        DeviceModel getDeviceById(int id);
        DeviceModel getDeviceByName(const QString& name);
        DeviceModel getDeviceByAddress(const QString& address);
        void insertDevice(const DeviceModel& model);
        void deleteDevice(int id);

        QList<LibraryModel> getLibraryMedia();
        QList<LibraryModel> getLibraryTemplate();
        QList<LibraryModel> getLibraryMediaByFilter(const QString& filter);
        QList<LibraryModel> getLibraryTemplateByFilter(const QString& filter);
        QList<LibraryModel> getLibraryByDeviceId(int deviceId);
        QList<LibraryModel> getLibraryMediaByDeviceAddress(const QString& address);
        QList<LibraryModel> getLibraryTemplateByDeviceAddress(const QString& address);
        QList<LibraryModel> getLibraryDataByDeviceAddress(const QString& name);
        void updateLibraryMedia(const QString& name, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void updateLibraryTemplate(const QString& name, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void updateLibraryData(const QString& name, const QList<LibraryModel>& deleteModels, const QList<LibraryModel>& insertModels);
        void deleteLibrary(int deviceId);

    private:
        QMutex mutex;
};
