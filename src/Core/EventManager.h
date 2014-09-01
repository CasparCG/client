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
#include "Events/CloseApplicationEvent.h"
#include "Events/ExportPresetMenuEvent.h"
#include "Events/SaveAsPresetMenuEvent.h"
#include "Events/ToggleFullscreenEvent.h"
#include "Events/Atem/AtemDeviceChangedEvent.h"
#include "Events/Inspector/AddTemplateDataEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Inspector/LabelChangedEvent.h"
#include "Events/Inspector/TargetChangedEvent.h"
#include "Events/Inspector/TemplateChangedEvent.h"
#include "Events/Inspector/VideolayerChangedEvent.h"
#include "Events/Inspector/ShowAddTemplateDataDialogEvent.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Library/AutoRefreshLibraryEvent.h"
#include "Events/Library/RefreshLibraryEvent.h"
#include "Events/Library/LibraryFilterChangedEvent.h"
#include "Events/Rundown/ActiveRundownChangedEvent.h"
#include "Events/Rundown/AddRudnownItemEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringMenuEvent.h"
#include "Events/Rundown/AutoPlayNextRundownItemEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"
#include "Events/Rundown/CloseRundownEvent.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Rundown/DeleteRundownEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/ExecutePlayoutCommandEvent.h"
#include "Events/Rundown/ExecuteRundownItemEvent.h"
#include "Events/Rundown/NewRundownEvent.h"
#include "Events/Rundown/NewRundownMenuEvent.h"
#include "Events/Rundown/MarkItemAsUsedEvent.h"
#include "Events/Rundown/MarkItemAsUnusedEvent.h"
#include "Events/Rundown/MarkAllItemsAsUsedEvent.h"
#include "Events/Rundown/MarkAllItemsAsUnusedEvent.h"
#include "Events/Rundown/OpenRundownEvent.h"
#include "Events/Rundown/OpenRundownFromUrlEvent.h"
#include "Events/Rundown/OpenRundownFromUrlMenuEvent.h"
#include "Events/Rundown/OpenRundownMenuEvent.h"
#include "Events/Rundown/SaveMenuEvent.h"
#include "Events/Rundown/SaveAsMenuEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringEvent.h"
#include "Events/Rundown/RemoveItemFromAutoPlayQueueEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Events/Rundown/ReloadRundownEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/Rundown/CopyItemPropertiesEvent.h"
#include "Events/Rundown/PasteItemPropertiesEvent.h"
#include "Events/TriCaster/TriCasterDeviceChangedEvent.h"
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

        Q_SIGNAL void tricasterDeviceChanged(const TriCasterDeviceChangedEvent&);
        Q_SIGNAL void atemDeviceChanged(const AtemDeviceChangedEvent&);
        Q_SIGNAL void removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&);
        Q_SIGNAL void executePlayoutCommand(const ExecutePlayoutCommandEvent&);
        Q_SIGNAL void deleteRundown(const DeleteRundownEvent&);
        Q_SIGNAL void reloadRundown(const ReloadRundownEvent&);
        Q_SIGNAL void openRundown(const OpenRundownEvent&);
        Q_SIGNAL void openRundownMenu(const OpenRundownMenuEvent&);
        Q_SIGNAL void openRundownFromUrl(const OpenRundownFromUrlEvent&);
        Q_SIGNAL void addTemplateData(const AddTemplateDataEvent&);
        Q_SIGNAL void saveRundown(const SaveRundownEvent&);
        Q_SIGNAL void oscOutputChanged(const OscOutputChangedEvent&);
        Q_SIGNAL void closeRundown(const CloseRundownEvent&);
        Q_SIGNAL void activeRundownChanged(const ActiveRundownChangedEvent&);
        Q_SIGNAL void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SIGNAL void libraryItemSelected(const LibraryItemSelectedEvent&);
        Q_SIGNAL void statusbar(const StatusbarEvent&);
        Q_SIGNAL void newRundown(const NewRundownEvent&);
        Q_SIGNAL void newRundownMenu(const NewRundownMenuEvent&);
        Q_SIGNAL void openRundownFromUrlMenu(const OpenRundownFromUrlMenuEvent&);
        Q_SIGNAL void dataChanged(const DataChangedEvent&);
        Q_SIGNAL void mediaChanged(const MediaChangedEvent&);
        Q_SIGNAL void templateChanged(const TemplateChangedEvent&);
        Q_SIGNAL void allowRemoteTriggeringMenu(const AllowRemoteTriggeringMenuEvent&);
        Q_SIGNAL void importPreset(const ImportPresetEvent&);
        Q_SIGNAL void exportPreset(const ExportPresetEvent&);
        Q_SIGNAL void emptyRundown(const EmptyRundownEvent&);
        Q_SIGNAL void presetChanged(const PresetChangedEvent&);
        Q_SIGNAL void refreshLibrary(const RefreshLibraryEvent&);
        Q_SIGNAL void autoRefreshLibrary(const AutoRefreshLibraryEvent&);
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
        Q_SIGNAL void allowRemoteTriggering(const AllowRemoteTriggeringEvent&);
        Q_SIGNAL void autoPlayRundownItem(const AutoPlayRundownItemEvent&);
        Q_SIGNAL void autoPlayChanged(const AutoPlayChangedEvent&);
        Q_SIGNAL void autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&);
        Q_SIGNAL void showAddTemplateDataDialog(const ShowAddTemplateDataDialogEvent&);
        Q_SIGNAL void markItemAsUsed(const MarkItemAsUsedEvent&);
        Q_SIGNAL void markItemAsUnused(const MarkItemAsUnusedEvent&);
        Q_SIGNAL void markAllItemsAsUsed(const MarkAllItemsAsUsedEvent&);
        Q_SIGNAL void markAllItemsAsUnused(const MarkAllItemsAsUnusedEvent&);
        Q_SIGNAL void closeApplication(const CloseApplicationEvent&);
        Q_SIGNAL void copyItemProperties(const CopyItemPropertiesEvent&);
        Q_SIGNAL void pasteItemProperties(const PasteItemPropertiesEvent&);
        Q_SIGNAL void libraryFilterChanged(const LibraryFilterChangedEvent&);
        Q_SIGNAL void exportPresetMenu(const ExportPresetMenuEvent&);
        Q_SIGNAL void saveAsPresetMenu(const SaveAsPresetMenuEvent&);
        Q_SIGNAL void toggleFullscreen(const ToggleFullscreenEvent&);
        Q_SIGNAL void saveMenu(const SaveMenuEvent&);
        Q_SIGNAL void saveAsMenu(const SaveAsMenuEvent&);

        void fireTriCasterDeviceChangedEvent(const TriCasterDeviceChangedEvent&);
        void fireAtemDeviceChangedEvent(const AtemDeviceChangedEvent&);
        void fireRemoveItemFromAutoPlayQueueEvent(const RemoveItemFromAutoPlayQueueEvent&);
        void fireExecutePlayoutCommandEvent(const ExecutePlayoutCommandEvent&);
        void fireReloadRundownEvent(const ReloadRundownEvent&);
        void fireOpenRundownEvent(const OpenRundownEvent&);
        void fireOpenRundownFromUrlMenuEvent(const OpenRundownFromUrlMenuEvent&);
        void fireAddTemplateDataEvent(const AddTemplateDataEvent&);
        void fireSaveRundownEvent(const SaveRundownEvent&);
        void fireOscOutputChangedEvent(const OscOutputChangedEvent&);
        void fireCloseRundownEvent(const CloseRundownEvent&);
        void fireActiveRundownChangedEvent(const ActiveRundownChangedEvent&);
        void fireRundownItemSelectedEvent(const RundownItemSelectedEvent&);
        void fireLibraryItemSelectedEvent(const LibraryItemSelectedEvent&);
        void fireDeleteRundownEvent(const DeleteRundownEvent&);
        void fireStatusbarEvent(const StatusbarEvent& event);
        void fireNewRundownEvent(const NewRundownEvent&);
        void fireNewRundownMenuEvent(const NewRundownMenuEvent&);
        void fireOpenRundownMenuEvent(const OpenRundownMenuEvent&);
        void fireOpenRundownFromUrlEvent(const OpenRundownFromUrlEvent&);
        void fireDataChangedEvent(const DataChangedEvent&);
        void fireMediaChangedEvent(const MediaChangedEvent&);
        void fireTemplateChangedEvent(const TemplateChangedEvent&);
        void fireAllowRemoteTriggeringMenuEvent(const AllowRemoteTriggeringMenuEvent&);
        void fireImportPresetEvent(const ImportPresetEvent&);
        void fireExportPresetEvent(const ExportPresetEvent&);
        void fireEmptyRundownEvent(const EmptyRundownEvent&);
        void firePresetChangedEvent(const PresetChangedEvent&);
        void fireRefreshLibraryEvent(const RefreshLibraryEvent&);
        void fireAutoRefreshLibraryEvent(const AutoRefreshLibraryEvent&);
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
        void fireAllowRemoteTriggeringEvent(const AllowRemoteTriggeringEvent& event);
        void fireAutoPlayRundownItemEvent(const AutoPlayRundownItemEvent&);
        void fireAutoPlayNextRundownItemEvent(const AutoPlayNextRundownItemEvent&);
        void fireShowAddTemplateDataDialogEvent(const ShowAddTemplateDataDialogEvent&);
        void fireMarkItemAsUsedEvent(const MarkItemAsUsedEvent&);
        void fireMarkItemAsUnusedEvent(const MarkItemAsUnusedEvent&);
        void fireMarkAllItemsAsUsedEvent(const MarkAllItemsAsUsedEvent&);
        void fireMarkAllItemsAsUnusedEvent(const MarkAllItemsAsUnusedEvent&);
        void fireCloseApplicationEvent(const CloseApplicationEvent&);
        void fireCopyItemPropertiesEvent(const CopyItemPropertiesEvent&);
        void firePasteItemPropertiesEvent(const PasteItemPropertiesEvent&);
        void fireLibraryFilterChangedEvent(const LibraryFilterChangedEvent&);
        void fireExportPresetMenuEvent(const ExportPresetMenuEvent&);
        void fireSaveAsPresetMenuEvent(const SaveAsPresetMenuEvent&);
        void fireToggleFullscreenEvent(const ToggleFullscreenEvent&);
        void fireSaveMenuEvent(const SaveMenuEvent&);
        void fireSaveAsMenuEvent(const SaveAsMenuEvent&);
};
