#pragma once

#include "Shared.h"

#include "Global.h"

#include "Commands/AbstractCommand.h"

#include "Events/AddPresetItemEvent.h"
#include "Events/DataChangedEvent.h"
#include "Events/ExportPresetEvent.h"
#include "Events/ImportPresetEvent.h"
#include "Events/MediaChangedEvent.h"
#include "Events/OscOutputChangedEvent.h"
#include "Events/PresetChangedEvent.h"
#include "Events/PreviewEvent.h"
#include "Events/SaveAsPresetEvent.h"
#include "Events/StatusbarEvent.h"
#include "Events/Inspector/AddTemplateDataEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Events/Inspector/VideolayerChangedEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Library/AutoRefreshLibraryEvent.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/AddRudnownItemEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/AutoPlayNextRundownItemEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"
#include "Events/Rundown/CloseRundownEvent.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Rundown/DeleteRundownEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/ExecuteRundownItemEvent.h"
#include "Events/Rundown/NewRundownEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/RemoteRundownTriggeringEvent.h"
#include "Events/Rundown/RundownIsChangedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Models/BlendModeModel.h"
#include "Models/LibraryModel.h"

#include <QtCore/QObject>

#include <QtGui/QWidget>
#include <QtGui/QTreeWidgetItem>

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
        void fireRefreshLibraryEvent(int delay = 0);
        void fireEmptyRundownEvent();
        void fireAddTemplateDataEvent(const QString& value, bool storedData);
        void fireStatusbarEvent(const QString& message, int timeout = 3000);
        void fireActiveRundownChangedEvent(const QString& path);
        void fireRundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model, QWidget* source = NULL, QWidget* parent = NULL);
        void fireLibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model);
        void fireAutoRefreshLibraryEvent(bool autoRefresh, int interval);
        void fireMediaChangedEvent();
        void fireTemplateChangedEvent();
        void fireDataChangedEvent();
        void firePresetChangedEvent();
        void fireSaveRundownEvent(bool saveAs);
        void fireOscOutputChangedEvent();
        void fireCloseRundownEvent();
        void fireAllowRemoteTriggeringMenuEvent(bool enabled);








        Q_SIGNAL void preview(const PreviewEvent&);
        Q_SIGNAL void channelChanged(const ChannelChangedEvent&);
        Q_SIGNAL void videolayerChanged(const VideolayerChangedEvent&);
        Q_SIGNAL void labelChanged(const LabelChangedEvent&);
        Q_SIGNAL void targetChanged(const TargetChangedEvent&);
        Q_SIGNAL void deviceChanged(const DeviceChangedEvent&);
        Q_SIGNAL void saveAsPreset(const SaveAsPresetEvent&);
        Q_SIGNAL void addPresetItem(const AddPresetItemEvent&);
        Q_SIGNAL void addRudnownItem(const AddRudnownItemEvent&);
        Q_SIGNAL void toggleCompactView(const CompactViewEvent&);
        Q_SIGNAL void executeRundownItem(const ExecuteRundownItemEvent&);
        Q_SIGNAL void remoteRundownTriggering(const RemoteRundownTriggeringEvent&);
        Q_SIGNAL void autoPlayRundownItem(const AutoPlayRundownItemEvent&);
        Q_SIGNAL void autoPlayChanged(const AutoPlayChangedEvent&);
        Q_SIGNAL void autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&);

        void firePreviewEvent(const PreviewEvent&);
        void fireChannelChangedEvent(const ChannelChangedEvent&);
        void fireVideolayerChangedEvent(const VideolayerChangedEvent&);
        void fireLabelChangedEvent(const LabelChangedEvent&);
        void fireTargetChangedEvent(const TargetChangedEvent&);
        void fireDeviceChangedEvent(const DeviceChangedEvent&);
        void fireAutoPlayChangedEvent(const AutoPlayChangedEvent&);
        void fireSaveAsPresetEvent(const SaveAsPresetEvent&);
        void fireAddPresetItemEvent(const AddPresetItemEvent& event);
        void fireAddRudnownItemEvent(const QString& type);
        void fireAddRudnownItemEvent(const LibraryModel& model);
        void fireToggleCompactViewEvent(const CompactViewEvent& event);
        void fireExecuteRundownItemEvent(const ExecuteRundownItemEvent& event);
        void fireRemoteRundownTriggeringEvent(const RemoteRundownTriggeringEvent& event);
        void fireAutoPlayRundownItemEvent(const AutoPlayRundownItemEvent&);
        void fireAutoPlayNextRundownItemEvent(const AutoPlayNextRundownItemEvent&);
};
