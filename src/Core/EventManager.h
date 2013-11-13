#pragma once

#include "Shared.h"

#include "Global.h"

#include "Commands/AbstractCommand.h"
#include "Events/Rundown/NewRundownEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/AddRudnownItemEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Library/AutoRefreshLibraryEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Events/PreviewEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/DeleteRundownEvent.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/VideolayerChangedEvent.h"
#include "Events/PresetChangedEvent.h"
#include "Events/AddPresetItemEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Events/Rundown/RundownIsChangedEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"
#include "Events/Rundown/AutoPlayNextRundownItemEvent.h"
#include "Events/Inspector/AddTemplateDataEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
#include "Models/BlendModeModel.h"
#include "Models/LibraryModel.h"

#include <QtCore/QObject>

#include <QtGui/QWidget>

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
        void fireRundownIsChangedEvent(bool changed);
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
        void fireAddTemplateDataEvent(const QString& value, bool storedData);
        void fireAddRudnownItemEvent(const QString& type);
        void fireAddPresetItemEvent(const QString& preset);
        void fireStatusbarEvent(const QString& message, int timeout = 3000);
        void fireActiveRundownChangedEvent(const QString& path);
        void fireRundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model, QWidget* source = NULL, QWidget* parent = NULL);
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
        void fireAutoPlayRundownItemEvent(QWidget* widget);
        void fireAutoPlayNextRundownItemEvent(QWidget* widget);
        void fireAutoPlayChangedEvent(bool autoPlay);
};
