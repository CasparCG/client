#include "EventManager.h"

#include <QtGui/QApplication>

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

void EventManager::fireInsertRepositoryChangesEvent(const InsertRepositoryChangesEvent& event)
{
    emit insertRepositoryChanges(event);
}

void EventManager::fireRepositoryRundownEvent(const RepositoryRundownEvent& event)
{
    emit repositoryRundown(event);
}

void EventManager::fireTriCasterDeviceChangedEvent(const TriCasterDeviceChangedEvent& event)
{
    emit tricasterDeviceChanged(event);
}

void EventManager::fireAtemDeviceChangedEvent(const AtemDeviceChangedEvent& event)
{
    emit atemDeviceChanged(event);
}

void EventManager::fireRemoveItemFromAutoPlayQueueEvent(const RemoveItemFromAutoPlayQueueEvent& event)
{
    emit removeItemFromAutoPlayQueue(event);
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

void EventManager::fireToggleCompactViewEvent(const CompactViewEvent& event)
{
    emit toggleCompactView(event);
}

void EventManager::fireExecuteRundownItemEvent(const ExecuteRundownItemEvent& event)
{
    emit executeRundownItem(event);
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
    else if (type == Rundown::CROP)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Crop", "", "", Rundown::CROP, 0, "")));
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
    else if (type == Rundown::GEOMETRY)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Transformation", "", "", Rundown::GEOMETRY, 0, "")));
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
    else if (type == Rundown::IMAGE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Image", "", "", Rundown::IMAGE, 0, "")));
    else if (type == Rundown::TEMPLATE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Template", "", "", Rundown::TEMPLATE, 0, "")));
    else if (type == Rundown::VIDEO)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Video", "", "", Rundown::VIDEO, 0, "")));
    else if (type == Rundown::CUSTOMCOMMAND)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Custom Command", "", "", Rundown::CUSTOMCOMMAND, 0, "")));
    else if (type ==  Rundown::TRICASTERINPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Select Input", "", "", Rundown::TRICASTERINPUT, 0, "")));
    else if (type ==  Rundown::TRICASTERPRESET)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Select Preset", "", "", Rundown::TRICASTERPRESET, 0, "")));
    else if (type ==  Rundown::TRICASTERAUTO)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Trigger Auto", "", "", Rundown::TRICASTERAUTO, 0, "")));
    else if (type ==  Rundown::TRICASTERTAKE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Trigger Take", "", "", Rundown::TRICASTERTAKE, 0, "")));
    else if (type ==  Rundown::TRICASTERNETSOURCE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Select Network Source", "", "", Rundown::TRICASTERNETSOURCE, 0, "")));
    else if (type ==  Rundown::TRICASTERMACRO)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Play Macro", "", "", Rundown::TRICASTERMACRO, 0, "")));
    else if (type ==  Rundown::OSCOUTPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "OSC Output", "", "", Rundown::OSCOUTPUT, 0, "")));
    else if (type ==  Rundown::ATEMINPUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Select Input", "", "", Rundown::ATEMINPUT, 0, "")));
    else if (type ==  Rundown::ATEMAUTO)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Trigger Auto", "", "", Rundown::ATEMAUTO, 0, "")));
    else if (type ==  Rundown::ATEMCUT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Trigger Cut", "", "", Rundown::ATEMCUT, 0, "")));
    else if (type ==  Rundown::ATEMKEYERSTATE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Set Keyer State", "", "", Rundown::ATEMKEYERSTATE, 0, "")));
    else if (type ==  Rundown::ATEMVIDEOFORMAT)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Set Video Format", "", "", Rundown::ATEMVIDEOFORMAT, 0, "")));
    else if (type ==  Rundown::ATEMAUDIOINPUTSTATE)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Set Audio Input State", "", "", Rundown::ATEMAUDIOINPUTSTATE, 0, "")));
    else if (type ==  Rundown::PLAYOUTCOMMAND)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Playout Command", "", "", Rundown::PLAYOUTCOMMAND, 0, "")));
    else if (type ==  Rundown::FADETOBLACK)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Fade to Black", "", "", Rundown::FADETOBLACK, 0, "")));
    else if (type ==  Rundown::PANASONICPRESET)
        emit addRudnownItem(AddRudnownItemEvent(LibraryModel(0, "Camera Preset", "", "", Rundown::PANASONICPRESET, 0, "")));
}
