cmake_minimum_required(VERSION 3.16)
project(widgets VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

qt_add_library(widgets
    STATIC
    AboutDialog.cpp AboutDialog.h AboutDialog.ui
    Action/ActionWidget.cpp Action/ActionWidget.h Action/ActionWidget.ui
    AudioLevelsWidget.cpp AudioLevelsWidget.h AudioLevelsWidget.ui
    AudioMeterWidget.cpp AudioMeterWidget.h AudioMeterWidget.ui
    DeviceDialog.cpp DeviceDialog.h DeviceDialog.ui
    DurationWidget.cpp DurationWidget.h DurationWidget.ui
    HelpDialog.cpp HelpDialog.h HelpDialog.ui
    ImportDeviceDialog.cpp ImportDeviceDialog.h ImportDeviceDialog.ui
    Inspector/HttpDataTreeBaseWidget.cpp Inspector/HttpDataTreeBaseWidget.h
    Inspector/InspectorAnchorWidget.cpp Inspector/InspectorAnchorWidget.h Inspector/InspectorAnchorWidget.ui
    Inspector/InspectorAudioWidget.cpp Inspector/InspectorAudioWidget.h Inspector/InspectorAudioWidget.ui
    Inspector/InspectorBlendModeWidget.cpp Inspector/InspectorBlendModeWidget.h Inspector/InspectorBlendModeWidget.ui
    Inspector/InspectorBrightnessWidget.cpp Inspector/InspectorBrightnessWidget.h Inspector/InspectorBrightnessWidget.ui
    Inspector/InspectorChromaWidget.cpp Inspector/InspectorChromaWidget.h Inspector/InspectorChromaWidget.ui
    Inspector/InspectorClearOutputWidget.cpp Inspector/InspectorClearOutputWidget.h Inspector/InspectorClearOutputWidget.ui
    Inspector/InspectorClipWidget.cpp Inspector/InspectorClipWidget.h Inspector/InspectorClipWidget.ui
    Inspector/InspectorContrastWidget.cpp Inspector/InspectorContrastWidget.h Inspector/InspectorContrastWidget.ui
    Inspector/InspectorCropWidget.cpp Inspector/InspectorCropWidget.h Inspector/InspectorCropWidget.ui
    Inspector/InspectorCustomCommandWidget.cpp Inspector/InspectorCustomCommandWidget.h Inspector/InspectorCustomCommandWidget.ui
    Inspector/InspectorDeckLinkInputWidget.cpp Inspector/InspectorDeckLinkInputWidget.h Inspector/InspectorDeckLinkInputWidget.ui
    Inspector/InspectorFadeToBlackWidget.cpp Inspector/InspectorFadeToBlackWidget.h Inspector/InspectorFadeToBlackWidget.ui
    Inspector/InspectorFileRecorderWidget.cpp Inspector/InspectorFileRecorderWidget.h Inspector/InspectorFileRecorderWidget.ui
    Inspector/InspectorFillWidget.cpp Inspector/InspectorFillWidget.h Inspector/InspectorFillWidget.ui
    Inspector/InspectorGpiOutputWidget.cpp Inspector/InspectorGpiOutputWidget.h Inspector/InspectorGpiOutputWidget.ui
    Inspector/InspectorGridWidget.cpp Inspector/InspectorGridWidget.h Inspector/InspectorGridWidget.ui
    Inspector/InspectorGroupWidget.cpp Inspector/InspectorGroupWidget.h Inspector/InspectorGroupWidget.ui
    Inspector/InspectorHtmlWidget.cpp Inspector/InspectorHtmlWidget.h Inspector/InspectorHtmlWidget.ui
    Inspector/InspectorHttpGetWidget.cpp Inspector/InspectorHttpGetWidget.h Inspector/InspectorHttpGetWidget.ui
    Inspector/InspectorHttpPostWidget.cpp Inspector/InspectorHttpPostWidget.h Inspector/InspectorHttpPostWidget.ui
    Inspector/InspectorImageScrollerWidget.cpp Inspector/InspectorImageScrollerWidget.h Inspector/InspectorImageScrollerWidget.ui
    Inspector/InspectorKeyerWidget.cpp Inspector/InspectorKeyerWidget.h Inspector/InspectorKeyerWidget.ui
    Inspector/InspectorLevelsWidget.cpp Inspector/InspectorLevelsWidget.h Inspector/InspectorLevelsWidget.ui
    Inspector/InspectorMetadataWidget.cpp Inspector/InspectorMetadataWidget.h Inspector/InspectorMetadataWidget.ui
    Inspector/InspectorMovieWidget.cpp Inspector/InspectorMovieWidget.h Inspector/InspectorMovieWidget.ui
    Inspector/InspectorOpacityWidget.cpp Inspector/InspectorOpacityWidget.h Inspector/InspectorOpacityWidget.ui
    Inspector/InspectorOscOutputWidget.cpp Inspector/InspectorOscOutputWidget.h Inspector/InspectorOscOutputWidget.ui
    Inspector/InspectorOutputWidget.cpp Inspector/InspectorOutputWidget.h Inspector/InspectorOutputWidget.ui
    Inspector/InspectorPerspectiveWidget.cpp Inspector/InspectorPerspectiveWidget.h Inspector/InspectorPerspectiveWidget.ui
    Inspector/InspectorPlayoutCommandWidget.cpp Inspector/InspectorPlayoutCommandWidget.h Inspector/InspectorPlayoutCommandWidget.ui
    Inspector/InspectorPrintWidget.cpp Inspector/InspectorPrintWidget.h Inspector/InspectorPrintWidget.ui
    Inspector/InspectorRotationWidget.cpp Inspector/InspectorRotationWidget.h Inspector/InspectorRotationWidget.ui
    Inspector/InspectorRouteChannelWidget.cpp Inspector/InspectorRouteChannelWidget.h Inspector/InspectorRouteChannelWidget.ui
    Inspector/InspectorRouteVideolayerWidget.cpp Inspector/InspectorRouteVideolayerWidget.h Inspector/InspectorRouteVideolayerWidget.ui
    Inspector/InspectorSaturationWidget.cpp Inspector/InspectorSaturationWidget.h Inspector/InspectorSaturationWidget.ui
    Inspector/InspectorSolidColorWidget.cpp Inspector/InspectorSolidColorWidget.h Inspector/InspectorSolidColorWidget.ui
    Inspector/InspectorStillWidget.cpp Inspector/InspectorStillWidget.h Inspector/InspectorStillWidget.ui
    Inspector/InspectorTemplateWidget.cpp Inspector/InspectorTemplateWidget.h Inspector/InspectorTemplateWidget.ui
    Inspector/InspectorVolumeWidget.cpp Inspector/InspectorVolumeWidget.h Inspector/InspectorVolumeWidget.ui
    Inspector/InspectorWidget.cpp Inspector/InspectorWidget.h Inspector/InspectorWidget.ui
    Inspector/KeyValueDialog.cpp Inspector/KeyValueDialog.h Inspector/KeyValueDialog.ui
    Inspector/TargetComboBaseWidget.cpp Inspector/TargetComboBaseWidget.h
    Inspector/TemplateDataTreeBaseWidget.cpp Inspector/TemplateDataTreeBaseWidget.h
    Library/AudioTreeBaseWidget.cpp Library/AudioTreeBaseWidget.h
    Library/DataTreeBaseWidget.cpp Library/DataTreeBaseWidget.h
    Library/DeviceFilterWidget.cpp Library/DeviceFilterWidget.h Library/DeviceFilterWidget.ui
    Library/ImageTreeBaseWidget.cpp Library/ImageTreeBaseWidget.h
    Library/LibraryWidget.cpp Library/LibraryWidget.h Library/LibraryWidget.ui
    Library/PresetTreeBaseWidget.cpp Library/PresetTreeBaseWidget.h
    Library/TemplateTreeBaseWidget.cpp Library/TemplateTreeBaseWidget.h
    Library/ToolTreeBaseWidget.cpp Library/ToolTreeBaseWidget.h
    Library/VideoTreeBaseWidget.cpp Library/VideoTreeBaseWidget.h
    LiveDialog.cpp LiveDialog.h LiveDialog.ui
    LiveWidget.cpp LiveWidget.h LiveWidget.ui
    MainWindow.cpp MainWindow.h MainWindow.ui
    OscOutputDialog.cpp OscOutputDialog.h OscOutputDialog.ui
    OscTimeWidget.cpp OscTimeWidget.h OscTimeWidget.ui
    PresetDialog.cpp PresetDialog.h PresetDialog.ui
    PreviewWidget.cpp PreviewWidget.h PreviewWidget.ui
    Rundown/AbstractRundownWidget.h
    Rundown/OpenRundownFromUrlDialog.cpp Rundown/OpenRundownFromUrlDialog.h Rundown/OpenRundownFromUrlDialog.ui
    Rundown/RundownAnchorWidget.cpp Rundown/RundownAnchorWidget.h Rundown/RundownAnchorWidget.ui
    Rundown/RundownAudioWidget.cpp Rundown/RundownAudioWidget.h Rundown/RundownAudioWidget.ui
    Rundown/RundownBlendModeWidget.cpp Rundown/RundownBlendModeWidget.h Rundown/RundownBlendModeWidget.ui
    Rundown/RundownBrightnessWidget.cpp Rundown/RundownBrightnessWidget.h Rundown/RundownBrightnessWidget.ui
    Rundown/RundownChromaWidget.cpp Rundown/RundownChromaWidget.h Rundown/RundownChromaWidget.ui
    Rundown/RundownClearOutputWidget.cpp Rundown/RundownClearOutputWidget.h Rundown/RundownClearOutputWidget.ui
    Rundown/RundownClipWidget.cpp Rundown/RundownClipWidget.h Rundown/RundownClipWidget.ui
    Rundown/RundownCommitWidget.cpp Rundown/RundownCommitWidget.h Rundown/RundownCommitWidget.ui
    Rundown/RundownContrastWidget.cpp Rundown/RundownContrastWidget.h Rundown/RundownContrastWidget.ui
    Rundown/RundownCropWidget.cpp Rundown/RundownCropWidget.h Rundown/RundownCropWidget.ui
    Rundown/RundownCustomCommandWidget.cpp Rundown/RundownCustomCommandWidget.h Rundown/RundownCustomCommandWidget.ui
    Rundown/RundownDeckLinkInputWidget.cpp Rundown/RundownDeckLinkInputWidget.h Rundown/RundownDeckLinkInputWidget.ui
    Rundown/RundownFadeToBlackWidget.cpp Rundown/RundownFadeToBlackWidget.h Rundown/RundownFadeToBlackWidget.ui
    Rundown/RundownFileRecorderWidget.cpp Rundown/RundownFileRecorderWidget.h Rundown/RundownFileRecorderWidget.ui
    Rundown/RundownFillWidget.cpp Rundown/RundownFillWidget.h Rundown/RundownFillWidget.ui
    Rundown/RundownGpiOutputWidget.cpp Rundown/RundownGpiOutputWidget.h Rundown/RundownGpiOutputWidget.ui
    Rundown/RundownGridWidget.cpp Rundown/RundownGridWidget.h Rundown/RundownGridWidget.ui
    Rundown/RundownGroupWidget.cpp Rundown/RundownGroupWidget.h Rundown/RundownGroupWidget.ui
    Rundown/RundownHtmlWidget.cpp Rundown/RundownHtmlWidget.h Rundown/RundownHtmlWidget.ui
    Rundown/RundownHttpGetWidget.cpp Rundown/RundownHttpGetWidget.h Rundown/RundownHttpGetWidget.ui
    Rundown/RundownHttpPostWidget.cpp Rundown/RundownHttpPostWidget.h Rundown/RundownHttpPostWidget.ui
    Rundown/RundownImageScrollerWidget.cpp Rundown/RundownImageScrollerWidget.h Rundown/RundownImageScrollerWidget.ui
    Rundown/RundownItemFactory.cpp Rundown/RundownItemFactory.h
    Rundown/RundownKeyerWidget.cpp Rundown/RundownKeyerWidget.h Rundown/RundownKeyerWidget.ui
    Rundown/RundownLevelsWidget.cpp Rundown/RundownLevelsWidget.h Rundown/RundownLevelsWidget.ui
    Rundown/RundownMovieWidget.cpp Rundown/RundownMovieWidget.h Rundown/RundownMovieWidget.ui
    Rundown/RundownOpacityWidget.cpp Rundown/RundownOpacityWidget.h Rundown/RundownOpacityWidget.ui
    Rundown/RundownOscOutputWidget.cpp Rundown/RundownOscOutputWidget.h Rundown/RundownOscOutputWidget.ui
    Rundown/RundownPerspectiveWidget.cpp Rundown/RundownPerspectiveWidget.h Rundown/RundownPerspectiveWidget.ui
    Rundown/RundownPlayoutCommandWidget.cpp Rundown/RundownPlayoutCommandWidget.h Rundown/RundownPlayoutCommandWidget.ui
    Rundown/RundownPrintWidget.cpp Rundown/RundownPrintWidget.h Rundown/RundownPrintWidget.ui
    Rundown/RundownResetWidget.cpp Rundown/RundownResetWidget.h Rundown/RundownResetWidget.ui
    Rundown/RundownRotationWidget.cpp Rundown/RundownRotationWidget.h Rundown/RundownRotationWidget.ui
    Rundown/RundownRouteChannelWidget.cpp Rundown/RundownRouteChannelWidget.h Rundown/RundownRouteChannelWidget.ui
    Rundown/RundownRouteVideolayerWidget.cpp Rundown/RundownRouteVideolayerWidget.h Rundown/RundownRouteVideolayerWidget.ui
    Rundown/RundownSaturationWidget.cpp Rundown/RundownSaturationWidget.h Rundown/RundownSaturationWidget.ui
    Rundown/RundownSeparatorWidget.cpp Rundown/RundownSeparatorWidget.h Rundown/RundownSeparatorWidget.ui
    Rundown/RundownSolidColorWidget.cpp Rundown/RundownSolidColorWidget.h Rundown/RundownSolidColorWidget.ui
    Rundown/RundownStillWidget.cpp Rundown/RundownStillWidget.h Rundown/RundownStillWidget.ui
    Rundown/RundownTemplateWidget.cpp Rundown/RundownTemplateWidget.h Rundown/RundownTemplateWidget.ui
    Rundown/RundownTreeBaseWidget.cpp Rundown/RundownTreeBaseWidget.h
    Rundown/RundownTreeWidget.cpp Rundown/RundownTreeWidget.h Rundown/RundownTreeWidget.ui
    Rundown/RundownVolumeWidget.cpp Rundown/RundownVolumeWidget.h Rundown/RundownVolumeWidget.ui
    Rundown/RundownWidget.cpp Rundown/RundownWidget.h Rundown/RundownWidget.ui
    SettingsDialog.cpp SettingsDialog.h SettingsDialog.ui
    Shared.h
    Utils/ItemScheduler.cpp Utils/ItemScheduler.h
)
target_include_directories(widgets PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../Common
    ../Caspar
    ../Common
    ../Core
    ../Gpi
    ../Osc
    ../Repository
    ../Web
)
add_external_dependencies(widgets)

target_compile_definitions(widgets PUBLIC
    WIDGETS_LIBRARY
)

target_include_directories(widgets PUBLIC
    ${LIBVLC_INCLUDE_DIR}
)

target_link_libraries(widgets PUBLIC
    caspar
    common
    core
    gpi
    osc
    repository
    web

    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Sql
    Qt::Widgets
)

# Resources:
set(Widgets_resource_files
    "Images/Add.png"
    "Images/AddHover.png"
    "Images/Anchor.png"
    "Images/AnchorSmall.png"
    "Images/AnchorSmallHover.png"
    "Images/ArrowDown.png"
    "Images/ArrowDownDisabled.png"
    "Images/ArrowLeft.png"
    "Images/ArrowLeftDisabled.png"
    "Images/ArrowRight.png"
    "Images/ArrowRightDisabled.png"
    "Images/ArrowUp.png"
    "Images/ArrowUpDisabled.png"
    "Images/Attention.png"
    "Images/Audio.png"
    "Images/AudioSmall.png"
    "Images/AudioSmallHover.png"
    "Images/AutoPlayGroup.png"
    "Images/AutoPlayItem.png"
    "Images/AutoStep.png"
    "Images/BlendMode.png"
    "Images/BlendModeSmall.png"
    "Images/BlendModeSmallHover.png"
    "Images/Brightness.png"
    "Images/BrightnessSmall.png"
    "Images/BrightnessSmallHover.png"
    "Images/CasparCG.png"
    "Images/CheckboxChecked.png"
    "Images/CheckboxCheckedHover.png"
    "Images/CheckboxCheckedPressed.png"
    "Images/CheckboxUnchecked.png"
    "Images/CheckboxUncheckedHover.png"
    "Images/CheckboxUncheckedPressed.png"
    "Images/Checkerboard.png"
    "Images/Chocolate.png"
    "Images/Chroma.png"
    "Images/ChromaSmall.png"
    "Images/Clear.png"
    "Images/ClearSmall.png"
    "Images/ClearSmallHover.png"
    "Images/Clients.png"
    "Images/Clip.png"
    "Images/ClipSmall.png"
    "Images/ClipSmallHover.png"
    "Images/Close.png"
    "Images/CloseHover.png"
    "Images/Commit.png"
    "Images/CommitSmall.png"
    "Images/CommitSmallHover.png"
    "Images/Contrast.png"
    "Images/ContrastSmall.png"
    "Images/ContrastSmallHover.png"
    "Images/Crop.png"
    "Images/CropSmall.png"
    "Images/CropSmallHover.png"
    "Images/CustomCommand.png"
    "Images/CustomCommandSmall.png"
    "Images/CustomCommandSmallHover.png"
    "Images/DarkKhaki.png"
    "Images/DarkSlateGray.png"
    "Images/DataSmall.png"
    "Images/DeckLinkProducer.png"
    "Images/DeckLinkProducerSmall.png"
    "Images/DeckLinkProducerSmallHover.png"
    "Images/DisableCommandOff.png"
    "Images/DisableCommandOn.png"
    "Images/Disconnected.png"
    "Images/Dropdown.png"
    "Images/DropdownHover.png"
    "Images/FadeToBlack.png"
    "Images/FadeToBlackSmall.png"
    "Images/FadeToBlackSmallHover.png"
    "Images/FileRecorder.png"
    "Images/FileRecorderSmall.png"
    "Images/FileRecorderSmallHover.png"
    "Images/Fill.png"
    "Images/FillSmall.png"
    "Images/FillSmallHover.png"
    "Images/GpiConnected.png"
    "Images/GpiDisconnected.png"
    "Images/GpiOutput.png"
    "Images/GpiOutputSmall.png"
    "Images/GpiOutputSmallHover.png"
    "Images/Grid.png"
    "Images/GridSmall.png"
    "Images/GridSmallHover.png"
    "Images/Group.png"
    "Images/GroupSmall.png"
    "Images/GroupSmallHover.png"
    "Images/Html.png"
    "Images/HtmlSmall.png"
    "Images/HtmlSmallHover.png"
    "Images/HttpGet.png"
    "Images/HttpGetSmall.png"
    "Images/HttpGetSmallHover.png"
    "Images/HttpPost.png"
    "Images/HttpPostSmall.png"
    "Images/HttpPostSmallHover.png"
    "Images/ImageScroller.png"
    "Images/ImageScrollerSmall.png"
    "Images/ImageScrollerSmallHover.png"
    "Images/Import.png"
    "Images/ImportHover.png"
    "Images/Information.png"
    "Images/KeyerSmall.png"
    "Images/KeyerSmallHover.png"
    "Images/Levels.png"
    "Images/LevelsSmall.png"
    "Images/LevelsSmallHover.png"
    "Images/Logo.png"
    "Images/LoopIconOverlay.png"
    "Images/LoopProgressBarOverlay.png"
    "Images/Maroon.png"
    "Images/MaroonLight.png"
    "Images/Mask.png"
    "Images/MasterVolumeOff.png"
    "Images/MasterVolumeOn.png"
    "Images/Mixer.png"
    "Images/MixerSmall.png"
    "Images/Movie.png"
    "Images/MovieSmall.png"
    "Images/MovieSmallHover.png"
    "Images/New.png"
    "Images/OliveDrab.png"
    "Images/Opacity.png"
    "Images/OpacitySmall.png"
    "Images/OpacitySmallHover.png"
    "Images/OscOutput.png"
    "Images/OscOutputSmall.png"
    "Images/OscOutputSmallHover.png"
    "Images/PauseProgressBarOverlay.png"
    "Images/Perspective.png"
    "Images/PerspectiveSmall.png"
    "Images/PerspectiveSmallHover.png"
    "Images/PlayProgressBarOverlay.png"
    "Images/PlayoutCommand.png"
    "Images/PlayoutCommandSmall.png"
    "Images/PlayoutCommandSmallHover.png"
    "Images/Preset.png"
    "Images/PresetSmall.png"
    "Images/PresetSmallHover.png"
    "Images/Preview.png"
    "Images/PreviewAlpha.png"
    "Images/PreviewAlphaHover.png"
    "Images/Print.png"
    "Images/RadiobuttonChecked.png"
    "Images/RadiobuttonCheckedHover.png"
    "Images/RadiobuttonCheckedPressed.png"
    "Images/RadiobuttonUnchecked.png"
    "Images/RadiobuttonUncheckedHover.png"
    "Images/RadiobuttonUncheckedPressed.png"
    "Images/RemoteTriggeringSmall.png"
    "Images/Remove.png"
    "Images/RemoveHover.png"
    "Images/Reset.png"
    "Images/ResetSmall.png"
    "Images/ResetSmallHover.png"
    "Images/Rotation.png"
    "Images/RotationSmall.png"
    "Images/RotationSmallHover.png"
    "Images/RouteChannel.png"
    "Images/RouteChannelSmall.png"
    "Images/RouteChannelSmallHover.png"
    "Images/RouteVideolayer.png"
    "Images/RouteVideolayerSmall.png"
    "Images/RouteVideolayerSmallHover.png"
    "Images/RoyalBlue.png"
    "Images/Rundown.png"
    "Images/Saturation.png"
    "Images/SaturationSmall.png"
    "Images/SaturationSmallHover.png"
    "Images/SeaGreen.png"
    "Images/SeparatorSmall.png"
    "Images/SeparatorSmallHover.png"
    "Images/ServerBig.png"
    "Images/ServerSmall.png"
    "Images/Sienna.png"
    "Images/Snapshot.png"
    "Images/SnapshotSmall.png"
    "Images/SnapshotSmallHover.png"
    "Images/SolidColor.png"
    "Images/SolidColorSmall.png"
    "Images/SolidColorSmallHover.png"
    "Images/SplashScreen.png"
    "Images/SplitterHorizontal.png"
    "Images/SplitterVertical.png"
    "Images/SteelBlue.png"
    "Images/Still.png"
    "Images/StillSmall.png"
    "Images/StillSmallHover.png"
    "Images/StreamNotAvailable.png"
    "Images/StreamNotAvailableSmall.png"
    "Images/TabSplitter.png"
    "Images/Template.png"
    "Images/TemplateSmall.png"
    "Images/TemplateSmallHover.png"
    "Images/TestConnection.png"
    "Images/Thumbnail.png"
    "Images/ToolbarSplitterHorizontal.png"
    "Images/ToolbarSplitterVertical.png"
    "Images/UngroupSmall.png"
    "Images/UngroupSmallHover.png"
    "Images/Volume.png"
    "Images/VolumeSmall.png"
    "Images/VolumeSmallHover.png"
)

qt_add_resources(widgets "Widgets"
    PREFIX
        "/Graphics"
    FILES
        ${Widgets_resource_files}
)
set(Widgets1_resource_files
    "Stylesheets/Curve/Default.css"
    "Stylesheets/Curve/Extended.css"
    "Stylesheets/Curve/Linux.css"
    "Stylesheets/Curve/Mac.css"
    "Stylesheets/Curve/Windows.css"
    "Stylesheets/Flat/Default.css"
    "Stylesheets/Flat/Extended.css"
    "Stylesheets/Flat/Linux.css"
    "Stylesheets/Flat/Mac.css"
    "Stylesheets/Flat/Windows.css"
)

qt_add_resources(widgets "Widgets1"
    PREFIX
        "/Appearances"
    FILES
        ${Widgets1_resource_files}
)
