cmake_minimum_required(VERSION 3.16)
project(core VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(core
    STATIC
    Animations/ActiveAnimation.cpp Animations/ActiveAnimation.h
    Commands/AbstractCommand.cpp Commands/AbstractCommand.h
    Commands/AbstractPlayoutCommand.cpp Commands/AbstractPlayoutCommand.h
    Commands/AbstractProperties.cpp Commands/AbstractProperties.h
    Commands/AnchorCommand.cpp Commands/AnchorCommand.h
    Commands/AudioCommand.cpp Commands/AudioCommand.h
    Commands/BlendModeCommand.cpp Commands/BlendModeCommand.h
    Commands/BrightnessCommand.cpp Commands/BrightnessCommand.h
    Commands/ChromaCommand.cpp Commands/ChromaCommand.h
    Commands/ClearOutputCommand.cpp Commands/ClearOutputCommand.h
    Commands/ClipCommand.cpp Commands/ClipCommand.h
    Commands/CommitCommand.cpp Commands/CommitCommand.h
    Commands/ContrastCommand.cpp Commands/ContrastCommand.h
    Commands/CropCommand.cpp Commands/CropCommand.h
    Commands/CustomCommand.cpp Commands/CustomCommand.h
    Commands/DeckLinkInputCommand.cpp Commands/DeckLinkInputCommand.h
    Commands/FadeToBlackCommand.cpp Commands/FadeToBlackCommand.h
    Commands/FileRecorderCommand.cpp Commands/FileRecorderCommand.h
    Commands/FillCommand.cpp Commands/FillCommand.h
    Commands/GpiOutputCommand.cpp Commands/GpiOutputCommand.h
    Commands/GridCommand.cpp Commands/GridCommand.h
    Commands/GroupCommand.cpp Commands/GroupCommand.h
    Commands/HtmlCommand.cpp Commands/HtmlCommand.h
    Commands/HttpGetCommand.cpp Commands/HttpGetCommand.h
    Commands/HttpPostCommand.cpp Commands/HttpPostCommand.h
    Commands/ImageScrollerCommand.cpp Commands/ImageScrollerCommand.h
    Commands/KeyerCommand.cpp Commands/KeyerCommand.h
    Commands/LevelsCommand.cpp Commands/LevelsCommand.h
    Commands/MovieCommand.cpp Commands/MovieCommand.h
    Commands/OpacityCommand.cpp Commands/OpacityCommand.h
    Commands/OscOutputCommand.cpp Commands/OscOutputCommand.h
    Commands/PerspectiveCommand.cpp Commands/PerspectiveCommand.h
    Commands/PlayoutCommand.cpp Commands/PlayoutCommand.h
    Commands/PrintCommand.cpp Commands/PrintCommand.h
    Commands/ResetCommand.cpp Commands/ResetCommand.h
    Commands/RotationCommand.cpp Commands/RotationCommand.h
    Commands/RouteChannelCommand.cpp Commands/RouteChannelCommand.h
    Commands/RouteVideolayerCommand.cpp Commands/RouteVideolayerCommand.h
    Commands/SaturationCommand.cpp Commands/SaturationCommand.h
    Commands/SeparatorCommand.cpp Commands/SeparatorCommand.h
    Commands/SolidColorCommand.cpp Commands/SolidColorCommand.h
    Commands/StillCommand.cpp Commands/StillCommand.h
    Commands/TemplateCommand.cpp Commands/TemplateCommand.h
    Commands/VolumeCommand.cpp Commands/VolumeCommand.h
    DatabaseManager.cpp DatabaseManager.h
    DeviceManager.cpp DeviceManager.h
    EventManager.cpp EventManager.h
    Events/Action/AddActionItemEvent.cpp Events/Action/AddActionItemEvent.h
    Events/AddPresetItemEvent.cpp Events/AddPresetItemEvent.h
    Events/CloseApplicationEvent.cpp Events/CloseApplicationEvent.h
    Events/ConnectionStateChangedEvent.cpp Events/ConnectionStateChangedEvent.h
    Events/DataChangedEvent.cpp Events/DataChangedEvent.h
    Events/DurationChangedEvent.cpp Events/DurationChangedEvent.h
    Events/ExportPresetEvent.cpp Events/ExportPresetEvent.h
    Events/ExportPresetMenuEvent.cpp Events/ExportPresetMenuEvent.h
    Events/ImportPresetEvent.cpp Events/ImportPresetEvent.h
    Events/Inspector/AddTemplateDataEvent.cpp Events/Inspector/AddTemplateDataEvent.h
    Events/Inspector/AutoPlayChangedEvent.cpp Events/Inspector/AutoPlayChangedEvent.h
    Events/Inspector/ChannelChangedEvent.cpp Events/Inspector/ChannelChangedEvent.h
    Events/Inspector/DeviceChangedEvent.cpp Events/Inspector/DeviceChangedEvent.h
    Events/Inspector/LabelChangedEvent.cpp Events/Inspector/LabelChangedEvent.h
    Events/Inspector/ShowAddHttpGetDataDialogEvent.cpp Events/Inspector/ShowAddHttpGetDataDialogEvent.h
    Events/Inspector/ShowAddHttpPostDataDialogEvent.cpp Events/Inspector/ShowAddHttpPostDataDialogEvent.h
    Events/Inspector/ShowAddTemplateDataDialogEvent.cpp Events/Inspector/ShowAddTemplateDataDialogEvent.h
    Events/Inspector/TargetChangedEvent.cpp Events/Inspector/TargetChangedEvent.h
    Events/Inspector/TemplateChangedEvent.cpp Events/Inspector/TemplateChangedEvent.h
    Events/Inspector/VideolayerChangedEvent.cpp Events/Inspector/VideolayerChangedEvent.h
    Events/Library/AutoRefreshLibraryEvent.cpp Events/Library/AutoRefreshLibraryEvent.h
    Events/Library/LibraryFilterChangedEvent.cpp Events/Library/LibraryFilterChangedEvent.h
    Events/Library/LibraryItemSelectedEvent.cpp Events/Library/LibraryItemSelectedEvent.h
    Events/Library/RefreshLibraryEvent.cpp Events/Library/RefreshLibraryEvent.h
    Events/MediaChangedEvent.cpp Events/MediaChangedEvent.h
    Events/OscOutputChangedEvent.cpp Events/OscOutputChangedEvent.h
    Events/PresetChangedEvent.cpp Events/PresetChangedEvent.h
    Events/PreviewEvent.cpp Events/PreviewEvent.h
    Events/Rundown/ActiveRundownChangedEvent.cpp Events/Rundown/ActiveRundownChangedEvent.h
    Events/Rundown/AddRudnownItemEvent.cpp Events/Rundown/AddRudnownItemEvent.h
    Events/Rundown/AllowRemoteTriggeringEvent.cpp Events/Rundown/AllowRemoteTriggeringEvent.h
    Events/Rundown/AllowRemoteTriggeringMenuEvent.cpp Events/Rundown/AllowRemoteTriggeringMenuEvent.h
    Events/Rundown/AutoPlayNextRundownItemEvent.cpp Events/Rundown/AutoPlayNextRundownItemEvent.h
    Events/Rundown/AutoPlayRundownItemEvent.cpp Events/Rundown/AutoPlayRundownItemEvent.h
    Events/Rundown/ClearCurrentPlayingItemEvent.cpp Events/Rundown/ClearCurrentPlayingItemEvent.h
    Events/Rundown/CloseRundownEvent.cpp Events/Rundown/CloseRundownEvent.h
    Events/Rundown/CompactViewEvent.cpp Events/Rundown/CompactViewEvent.h
    Events/Rundown/CopyItemPropertiesEvent.cpp Events/Rundown/CopyItemPropertiesEvent.h
    Events/Rundown/CurrentItemChangedEvent.cpp Events/Rundown/CurrentItemChangedEvent.h
    Events/Rundown/DeleteRundownEvent.cpp Events/Rundown/DeleteRundownEvent.h
    Events/Rundown/EmptyRundownEvent.cpp Events/Rundown/EmptyRundownEvent.h
    Events/Rundown/ExecutePlayoutCommandEvent.cpp Events/Rundown/ExecutePlayoutCommandEvent.h
    Events/Rundown/ExecuteRundownItemEvent.cpp Events/Rundown/ExecuteRundownItemEvent.h
    Events/Rundown/InsertRepositoryChangesEvent.cpp Events/Rundown/InsertRepositoryChangesEvent.h
    Events/Rundown/MarkAllItemsAsUnusedEvent.cpp Events/Rundown/MarkAllItemsAsUnusedEvent.h
    Events/Rundown/MarkAllItemsAsUsedEvent.cpp Events/Rundown/MarkAllItemsAsUsedEvent.h
    Events/Rundown/MarkItemAsUnusedEvent.cpp Events/Rundown/MarkItemAsUnusedEvent.h
    Events/Rundown/MarkItemAsUsedEvent.cpp Events/Rundown/MarkItemAsUsedEvent.h
    Events/Rundown/NewRundownEvent.cpp Events/Rundown/NewRundownEvent.h
    Events/Rundown/NewRundownMenuEvent.cpp Events/Rundown/NewRundownMenuEvent.h
    Events/Rundown/OpenRundownEvent.cpp Events/Rundown/OpenRundownEvent.h
    Events/Rundown/OpenRundownFromUrlEvent.cpp Events/Rundown/OpenRundownFromUrlEvent.h
    Events/Rundown/OpenRundownFromUrlMenuEvent.cpp Events/Rundown/OpenRundownFromUrlMenuEvent.h
    Events/Rundown/OpenRundownMenuEvent.cpp Events/Rundown/OpenRundownMenuEvent.h
    Events/Rundown/PasteItemPropertiesEvent.cpp Events/Rundown/PasteItemPropertiesEvent.h
    Events/Rundown/ReloadRundownEvent.cpp Events/Rundown/ReloadRundownEvent.h
    Events/Rundown/ReloadRundownMenuEvent.cpp Events/Rundown/ReloadRundownMenuEvent.h
    Events/Rundown/RemoveItemFromAutoPlayQueueEvent.cpp Events/Rundown/RemoveItemFromAutoPlayQueueEvent.h
    Events/Rundown/RepositoryRundownEvent.cpp Events/Rundown/RepositoryRundownEvent.h
    Events/Rundown/RundownItemSelectedEvent.cpp Events/Rundown/RundownItemSelectedEvent.h
    Events/Rundown/SaveAsMenuEvent.cpp Events/Rundown/SaveAsMenuEvent.h
    Events/Rundown/SaveMenuEvent.cpp Events/Rundown/SaveMenuEvent.h
    Events/Rundown/SaveRundownEvent.cpp Events/Rundown/SaveRundownEvent.h
    Events/SaveAsPresetEvent.cpp Events/SaveAsPresetEvent.h
    Events/SaveAsPresetMenuEvent.cpp Events/SaveAsPresetMenuEvent.h
    Events/StatusbarEvent.cpp Events/StatusbarEvent.h
    Events/ToggleFullscreenEvent.cpp Events/ToggleFullscreenEvent.h
    GpiManager.cpp GpiManager.h
    LibraryManager.cpp LibraryManager.h
    Models/BlendModeModel.cpp Models/BlendModeModel.h
    Models/ChromaModel.cpp Models/ChromaModel.h
    Models/ConfigurationModel.cpp Models/ConfigurationModel.h
    Models/DeviceModel.cpp Models/DeviceModel.h
    Models/DirectionModel.cpp Models/DirectionModel.h
    Models/FormatModel.cpp Models/FormatModel.h
    Models/GpiModel.cpp Models/GpiModel.h
    Models/GpiPortModel.cpp Models/GpiPortModel.h
    Models/GpoPortModel.cpp Models/GpoPortModel.h
    Models/KeyValueModel.cpp Models/KeyValueModel.h
    Models/LibraryModel.cpp Models/LibraryModel.h
    Models/OscFileModel.cpp Models/OscFileModel.h
    Models/OscOutputModel.cpp Models/OscOutputModel.h
    Models/PresetModel.cpp Models/PresetModel.h
    Models/RundownModel.cpp Models/RundownModel.h
    Models/ThumbnailModel.cpp Models/ThumbnailModel.h
    Models/TransitionModel.cpp Models/TransitionModel.h
    Models/TweenModel.cpp Models/TweenModel.h
    Models/TypeModel.cpp Models/TypeModel.h
    OscDeviceManager.cpp OscDeviceManager.h
    OscSubscription.cpp OscSubscription.h
    OscWebSocketManager.cpp OscWebSocketManager.h
    Shared.h
    ThumbnailWorker.cpp ThumbnailWorker.h
)
add_external_dependencies(core)
target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../Common
    ../Caspar
    ../Common
    ../Gpi
    ../Osc
)

target_compile_definitions(core PUBLIC
    CORE_LIBRARY
)

target_link_libraries(core PUBLIC
    caspar
    common
    gpi
    osc
    
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Sql
    Qt::Widgets
)

# Resources:
set(Core_resource_files
    "Sql/ChangeScript-208.sql"
    "Sql/ChangeScript-209.sql"
    "Sql/ChangeScript-210.sql"
    "Sql/ChangeScript-211.sql"
    "Sql/ChangeScript-212.sql"
    "Sql/ChangeScript-213.sql"
    "Sql/ChangeScript-214.sql"
    "Sql/ChangeScript-215.sql"
    "Sql/ChangeScript-216.sql"
    "Sql/Schema.sql"
)

qt_add_resources(core "Core"
    PREFIX
        "/Scripts"
    FILES
        ${Core_resource_files}
)
