#pragma once

#include "Shared.h"

#include "Global.h"

#include "Commands/AbstractCommand.h"
#include "Events/NewRundownEvent.h"
#include "Events/NewRundownMenuEvent.h"
#include "Events/OpenRundownEvent.h"
#include "Events/SaveRundownEvent.h"
#include "Events/CompactViewEvent.h"
#include "Events/RefreshLibraryEvent.h"
#include "Events/EmptyRundownEvent.h"
#include "Events/AddRudnownItemEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/ActiveRundownChangedEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/AutoRefreshLibraryEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/TemplateChangedEvent.h"
#include "Events/PreviewEvent.h"
#include "Events/LabelChangedEvent.h"
#include "Events/TargetChangedEvent.h"
#include "Events/DeviceChangedEvent.h"
#include "Events/DeleteRundownEvent.h"
#include "Events/ChannelChangedEvent.h"
#include "Events/VideolayerChangedEvent.h"
#include "Events/PresetChangedEvent.h"
#include "Events/AddPresetItemEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Models/BlendModeModel.h"
#include "Models/LibraryModel.h"

#include <QtCore/QObject>

class CORE_EXPORT EventManager : public QObject
{
    Q_OBJECT

    public:
        explicit EventManager();

        static EventManager& getInstance();

        void initialize();
        void uninitialize();

        void fireProcessEvent();
        void fireNewRundownEvent();
        void fireImportPresetEvent();
        void fireExportPresetEvent();
        void fireNewRundownMenuEvent(bool enabled);
        void fireDeleteRundownEvent(int index);
        void fireOpenRundownEvent();
        void fireSaveRundownEvent(bool saveAs);
        void fireExecuteStopEvent();
        void fireExecutePlayEvent();
        void fireExecutePauseEvent();
        void fireExecuteLoadEvent();
        void fireExecuteNextEvent();
        void fireExecuteUpdateEvent();
        void fireExecuteInvokeEvent();
        void fireExecuteClearEvent();
        void fireExecuteClearVideolayerEvent();
        void fireExecuteClearChannelEvent();
        void fireToggleCompactViewEvent();
        void fireRefreshLibraryEvent(int delay = 0);
        void fireEmptyRundownEvent();
        void fireAddRudnownItemEvent(const LibraryModel& model);
        void fireAddRudnownItemEvent(const QString& type);
        void fireAddPresetItemEvent(const QString& preset);
        void fireStatusbarEvent(const QString& message, int timeout = 3000);
        void fireActiveRundownChangedEvent(const QString& path);
        void fireRundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model);
        void fireLibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model);
        void fireAutoRefreshLibraryEvent(bool autoRefresh, int interval);
        void fireMediaChangedEvent();
        void fireTemplateChangedEvent();
        void fireDataChangedEvent();
        void firePresetChangedEvent();
        void firePreviewEvent();
        void fireLabelChangedEvent(const QString& label);
        void fireTargetChangedEvent(const QString& target);
        void fireDeviceChangedEvent(const QString& deviceName);
        void fireChannelChangedEvent(int channel);
        void fireVideolayerChangedEvent(int videolayer);
};
