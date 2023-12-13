#include "EventManager.h"

#include <QtWidgets/QApplication>

Q_GLOBAL_STATIC(EventManager, eventManager)

EventManager::EventManager()
{
}

EventManager& EventManager::getInstance()
{
    return *eventManager();
}

void EventManager::initialize()
{
}

void EventManager::uninitialize()
{
}

void EventManager::fireClearDelayedCommands()
{
    emit clearDelayedCommands();
}

void EventManager::fireInsertRepositoryChangesEvent(const InsertRepositoryChangesEvent& event)
{
    emit insertRepositoryChanges(event);
}

void EventManager::fireRepositoryRundownEvent(const RepositoryRundownEvent& event)
{
    emit repositoryRundown(event);
}

void EventManager::fireRemoveItemFromAutoPlayQueueEvent(const RemoveItemFromAutoPlayQueueEvent& event)
{
    emit removeItemFromAutoPlayQueue(event);
}

void EventManager::fireClearCurrentPlayingItemEvent(const ClearCurrentPlayingItemEvent& event)
{
    emit clearCurrentPlayingItem(event);
}

void EventManager::fireExecutePlayoutCommandEvent(const ExecutePlayoutCommandEvent& event)
{
    emit executePlayoutCommand(event);
}

void EventManager::fireOpenRundownEvent(const OpenRundownEvent& event)
{
    emit openRundown(event);
}

void EventManager::fireOpenRundownMenuEvent(const OpenRundownMenuEvent& event)
{
    emit openRundownMenu(event);
}

void EventManager::fireDurationChangedEvent(const DurationChangedEvent& event)
{
    emit durationChanged(event);
}

void EventManager::fireReloadRundownMenuEvent(const ReloadRundownMenuEvent& event)
{
    emit reloadRundownMenu(event);
}

void EventManager::fireExportPresetMenuEvent(const ExportPresetMenuEvent& event)
{
    emit exportPresetMenu(event);
}

void EventManager::fireSaveAsPresetMenuEvent(const SaveAsPresetMenuEvent& event)
{
    emit saveAsPresetMenu(event);
}

void EventManager::fireSaveMenuEvent(const SaveMenuEvent& event)
{
    emit saveMenu(event);
}

void EventManager::fireSaveAsMenuEvent(const SaveAsMenuEvent& event)
{
    emit saveAsMenu(event);
}

void EventManager::fireToggleFullscreenEvent(const ToggleFullscreenEvent& event)
{
    emit toggleFullscreen(event);
}

void EventManager::fireOpenRundownFromUrlEvent(const OpenRundownFromUrlEvent& event)
{
    emit openRundownFromUrl(event);
}

void EventManager::fireSaveRundownEvent(const SaveRundownEvent& event)
{
    emit saveRundown(event);
}

void EventManager::fireCloseRundownEvent(const CloseRundownEvent& event)
{
    emit closeRundown(event);
}

void EventManager::fireReloadRundownEvent(const ReloadRundownEvent& event)
{
    emit reloadRundown(event);
}

void EventManager::fireAddTemplateDataEvent(const AddTemplateDataEvent& event)
{
    emit addTemplateData(event);
}

void EventManager::fireShowAddTemplateDataDialogEvent(const ShowAddTemplateDataDialogEvent& event)
{
    emit showAddTemplateDataDialog(event);
}

void EventManager::fireShowAddHttpGetDataDialogEvent(const ShowAddHttpGetDataDialogEvent& event)
{
    emit showAddHttpGetDataDialog(event);
}

void EventManager::fireShowAddHttpPostDataDialogEvent(const ShowAddHttpPostDataDialogEvent& event)
{
    emit showAddHttpPostDataDialog(event);
}

void EventManager::fireCurrentItemChangedEvent(const CurrentItemChangedEvent& event)
{
    emit currentItemChanged(event);
}

void EventManager::fireMarkItemAsUsedEvent(const MarkItemAsUsedEvent& event)
{
    emit markItemAsUsed(event);
}

void EventManager::fireMarkItemAsUnusedEvent(const MarkItemAsUnusedEvent& event)
{
    emit markItemAsUnused(event);
}

void EventManager::fireMarkAllItemsAsUsedEvent(const MarkAllItemsAsUsedEvent& event)
{
    emit markAllItemsAsUsed(event);
}

void EventManager::fireMarkAllItemsAsUnusedEvent(const MarkAllItemsAsUnusedEvent& event)
{
    emit markAllItemsAsUnused(event);
}

void EventManager::fireOscOutputChangedEvent(const OscOutputChangedEvent& event)
{
    emit oscOutputChanged(event);
}

void EventManager::fireRundownItemSelectedEvent(const RundownItemSelectedEvent& event)
{
    emit rundownItemSelected(event);
}

void EventManager::fireLibraryItemSelectedEvent(const LibraryItemSelectedEvent& event)
{
    emit libraryItemSelected(event);
}

void EventManager::fireDeleteRundownEvent(const DeleteRundownEvent& event)
{
    emit deleteRundown(event);
}

void EventManager::fireStatusbarEvent(const StatusbarEvent& event)
{
    emit statusbar(event);
}

void EventManager::fireActiveRundownChangedEvent(const ActiveRundownChangedEvent& event)
{
    emit activeRundownChanged(event);
}

void EventManager::fireNewRundownEvent(const NewRundownEvent& event)
{
    emit newRundown(event);
}

void EventManager::fireNewRundownMenuEvent(const NewRundownMenuEvent& event)
{
    emit newRundownMenu(event);
}

void EventManager::fireOpenRundownFromUrlMenuEvent(const OpenRundownFromUrlMenuEvent& event)
{
    emit openRundownFromUrlMenu(event);
}

void EventManager::fireAllowRemoteTriggeringMenuEvent(const AllowRemoteTriggeringMenuEvent& event)
{
    emit allowRemoteTriggeringMenu(event);
}

void EventManager::fireLibraryFilterChangedEvent(const LibraryFilterChangedEvent& event)
{
    emit libraryFilterChanged(event);
}

void EventManager::fireMediaChangedEvent(const MediaChangedEvent& event)
{
    emit mediaChanged(event);
}

void EventManager::fireTemplateChangedEvent(const TemplateChangedEvent& event)
{
    emit templateChanged(event);
}

void EventManager::fireDataChangedEvent(const DataChangedEvent& event)
{
    emit dataChanged(event);
}

void EventManager::fireImportPresetEvent(const ImportPresetEvent& event)
{
    emit importPreset(event);
}

void EventManager::fireExportPresetEvent(const ExportPresetEvent& event)
{
    emit exportPreset(event);
}

void EventManager::fireEmptyRundownEvent(const EmptyRundownEvent& event)
{
    emit emptyRundown(event);
}

void EventManager::firePresetChangedEvent(const PresetChangedEvent& event)
{
    emit presetChanged(event);
}

void EventManager::fireRefreshLibraryEvent(const RefreshLibraryEvent& event)
{
    emit refreshLibrary(event);
}

void EventManager::fireAutoRefreshLibraryEvent(const AutoRefreshLibraryEvent& event)
{
    emit autoRefreshLibrary(event);
}

void EventManager::firePreviewEvent(const PreviewEvent& event)
{
    emit preview(event);
}

void EventManager::fireChannelChangedEvent(const ChannelChangedEvent& event)
{
    emit channelChanged(event);
}

void EventManager::fireVideolayerChangedEvent(const VideolayerChangedEvent& event)
{
    emit videolayerChanged(event);
}

void EventManager::fireLabelChangedEvent(const LabelChangedEvent& event)
{
    emit labelChanged(event);
}

void EventManager::fireTargetChangedEvent(const TargetChangedEvent& event)
{
    emit targetChanged(event);
}

void EventManager::fireDeviceChangedEvent(const DeviceChangedEvent& event)
{
    emit deviceChanged(event);
}

void EventManager::fireAutoPlayNextRundownItemEvent(const AutoPlayNextRundownItemEvent& event)
{
    emit autoPlayNextRundownItem(event);
}

void EventManager::fireAutoPlayChangedEvent(const AutoPlayChangedEvent& event)
{
    emit autoPlayChanged(event);
}

void EventManager::fireSaveAsPresetEvent(const SaveAsPresetEvent& event)
{
    emit saveAsPreset(event);
}

void EventManager::fireAutoPlayRundownItemEvent(const AutoPlayRundownItemEvent& event)
{
    emit autoPlayRundownItem(event);
}

void EventManager::fireAddPresetItemEvent(const AddPresetItemEvent& event)
{
    emit addPresetItem(event);
}

void EventManager::fireExecuteRundownItemEvent(const ExecuteRundownItemEvent& event)
{
    emit executeRundownItem(event);
}

void EventManager::fireCompactViewEvent(const CompactViewEvent& event)
{
    emit compactView(event);
}

void EventManager::fireAllowRemoteTriggeringEvent(const AllowRemoteTriggeringEvent& event)
{
    emit allowRemoteTriggering(event);
}

void EventManager::fireAddRudnownItemEvent(const LibraryModel& model)
{
    emit addRudnownItem(AddRudnownItemEvent(model));
}

void EventManager::fireCloseApplicationEvent(const CloseApplicationEvent& event)
{
    emit closeApplication(CloseApplicationEvent(event));
}

void EventManager::fireCopyItemPropertiesEvent(const CopyItemPropertiesEvent& event)
{
    emit copyItemProperties(CopyItemPropertiesEvent(event));
}

void EventManager::firePasteItemPropertiesEvent(const PasteItemPropertiesEvent& event)
{
    emit pasteItemProperties(PasteItemPropertiesEvent(event));
}

void EventManager::fireAddRudnownItemEvent(const QString& type)
{
    if (type == Rundown::BLENDMODE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Blend Mode", "", "", Rundown::BLENDMODE, 0, "")));
    else if (type == Rundown::BRIGHTNESS)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Brightness", "", "", Rundown::BRIGHTNESS, 0, "")));
    else if (type == Rundown::CONTRAST)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Contrast", "", "", Rundown::CONTRAST, 0, "")));
    else if (type == Rundown::CLIP)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Clipping", "", "", Rundown::CLIP, 0, "")));
    else if (type == Rundown::CHROMAKEY)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Chroma Key", "", "", Rundown::CHROMAKEY, 0, "")));
    else if (type == Rundown::IMAGESCROLLER)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Image Scroller", "", "", Rundown::IMAGESCROLLER, 0, "")));
    else if (type == Rundown::DECKLINKINPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "DeckLink Input", "", "", Rundown::DECKLINKINPUT, 0, "")));
    else if (type == Rundown::PRINT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Channel Snapshot", "", "", Rundown::PRINT, 0, "")));
    else if (type == Rundown::CLEAROUTPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Clear Output", "", "", Rundown::CLEAROUTPUT, 0, "")));
    else if (type == Rundown::FILL)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Transform", "", "", Rundown::FILL, 0, "")));
    else if (type == Rundown::GPIOUTPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "GPI Output", "", "", Rundown::GPIOUTPUT, 0, "")));
    else if (type == Rundown::FILERECORDER)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "File Recorder", "", "", Rundown::FILERECORDER, 0, "")));
    else if (type == Rundown::SEPARATOR)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "", "", "", Rundown::SEPARATOR, 0, "")));
    else if (type == Rundown::GRID)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Grid", "", "", Rundown::GRID, 0, "")));
    else if (type == Rundown::SOLIDCOLOR)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Solid Color", "", "", Rundown::SOLIDCOLOR, 0, "")));
    else if (type == Rundown::KEYER)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Mask", "", "", Rundown::KEYER, 0, "")));
    else if (type == Rundown::LEVELS)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Levels", "", "", Rundown::LEVELS, 0, "")));
    else if (type == Rundown::OPACITY)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Opacity", "", "", Rundown::OPACITY, 0, "")));
    else if (type == Rundown::SATURATION)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Saturation", "", "", Rundown::SATURATION, 0, "")));
    else if (type == Rundown::VOLUME)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Volume", "", "", Rundown::VOLUME, 0, "")));
    else if (type == Rundown::COMMIT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Commit", "", "", Rundown::COMMIT, 0, "")));
    else if (type == Rundown::AUDIO)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Audio", "", "", Rundown::AUDIO, 0, "")));
    else if (type == Rundown::STILL)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Image", "", "", Rundown::STILL, 0, "")));
    else if (type == Rundown::TEMPLATE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Template", "", "", Rundown::TEMPLATE, 0, "")));
    else if (type == Rundown::MOVIE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Video", "", "", Rundown::MOVIE, 0, "")));
    else if (type == Rundown::CUSTOMCOMMAND)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Custom Command", "", "", Rundown::CUSTOMCOMMAND, 0, "")));
    else if (type ==  Rundown::OSCOUTPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "OSC Output", "", "", Rundown::OSCOUTPUT, 0, "")));
    else if (type ==  Rundown::PLAYOUTCOMMAND)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Playout Command", "", "", Rundown::PLAYOUTCOMMAND, 0, "")));
    else if (type ==  Rundown::FADETOBLACK)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Fade Out", "", "", Rundown::FADETOBLACK, 0, "")));
    else if (type ==  Rundown::PERSPECTIVE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Distort", "", "", Rundown::PERSPECTIVE, 0, "")));
    else if (type ==  Rundown::ROTATION)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Rotation", "", "", Rundown::ROTATION, 0, "")));
    else if (type ==  Rundown::ANCHOR)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Anchor Point", "", "", Rundown::ANCHOR, 0, "")));
    else if (type ==  Rundown::CROP)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Crop", "", "", Rundown::CROP, 0, "")));
    else if (type ==  Rundown::HTTPGET)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "HTTP GET Request", "", "", Rundown::HTTPGET, 0, "")));
    else if (type ==  Rundown::HTTPPOST)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "HTTP POST Request", "", "", Rundown::HTTPPOST, 0, "")));
    else if (type ==  Rundown::RESET)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Reset", "", "", Rundown::RESET, 0, "")));
    else if (type ==  Rundown::HTML)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "HTML Page", "", "", Rundown::HTML, 0, "")));
    else if (type ==  Rundown::ROUTECHANNEL)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Route Channel", "", "", Rundown::ROUTECHANNEL, 0, "")));
    else if (type ==  Rundown::ROUTEVIDEOLAYER)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Route Videolayer", "", "", Rundown::ROUTEVIDEOLAYER, 0, "")));
}
