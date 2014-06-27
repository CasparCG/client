#-------------------------------------------------
#
# Project created by QtCreator 2011-04-07T13:50:44
#
#-------------------------------------------------

QT += core gui sql network

TARGET = widgets
TEMPLATE = lib

DEFINES += WIDGETS_LIBRARY

HEADERS += \
    SettingsDialog.h \
    MainWindow.h \
    PreviewWidget.h \
    AboutDialog.h \
    Shared.h \
    HelpDialog.h \
    Inspector/InspectorWidget.h \
    Inspector/InspectorTemplateWidget.h \
    Inspector/InspectorOutputWidget.h \
    Inspector/InspectorMetadataWidget.h \
    Inspector/InspectorLevelsWidget.h \
    Inspector/InspectorGeometryWidget.h \
    Library/LibraryWidget.h \
    Rundown/RundownGroupWidget.h \
    Rundown/RundownTemplateWidget.h \
    Rundown/RundownWidget.h \
    Rundown/AbstractRundownWidget.h \
    Rundown/RundownCropWidget.h \
    Inspector/InspectorCropWidget.h \
    Rundown/RundownGeometryWidget.h \
    Inspector/InspectorBrightnessWidget.h \
    Rundown/RundownBrightnessWidget.h \
    Inspector/InspectorSaturationWidget.h \
    Rundown/RundownSaturationWidget.h \
    Rundown/RundownOpacityWidget.h \
    Inspector/InspectorOpacityWidget.h \
    Rundown/RundownContrastWidget.h \
    Inspector/InspectorContrastWidget.h \
    Rundown/RundownVolumeWidget.h \
    Inspector/InspectorVolumeWidget.h \
    Rundown/RundownLevelsWidget.h \
    Rundown/RundownKeyerWidget.h \
    Rundown/RundownGridWidget.h \
    Inspector/InspectorGridWidget.h \
    Rundown/RundownGpiOutputWidget.h \
    Rundown/RundownCommitWidget.h \
    Inspector/InspectorGpiOutputWidget.h \
    Rundown/RundownImageScrollerWidget.h \
    Inspector/InspectorImageScrollerWidget.h \
    Rundown/RundownFileRecorderWidget.h \
    Inspector/InspectorFileRecorderWidget.h \
    Inspector/InspectorBlendModeWidget.h \
    Rundown/RundownBlendModeWidget.h \
    Rundown/RundownSeparatorWidget.h \
    Rundown/RundownPrintWidget.h \
    Inspector/InspectorKeyerWidget.h \
    Inspector/InspectorPrintWidget.h \
    Rundown/RundownClearOutputWidget.h \
    Inspector/InspectorClearOutputWidget.h \
    Inspector/InspectorGroupWidget.h \
    Rundown/RundownAudioWidget.h \
    Inspector/InspectorAudioWidget.h \
    Rundown/RundownDeckLinkInputWidget.h \
    Inspector/InspectorDeckLinkInputWidget.h \
    Rundown/RundownSolidColorWidget.h \
    Inspector/InspectorSolidColorWidget.h \
    Rundown/RundownImageWidget.h \
    Inspector/InspectorImageWidget.h \
    Inspector/InspectorVideoWidget.h \
    Rundown/RundownVideoWidget.h \
    Rundown/RundownTreeWidget.h \
    Rundown/RundownItemFactory.h \
    DeviceDialog.h \
    PresetDialog.h \
    Rundown/RundownCustomCommandWidget.h \
    Inspector/InspectorCustomCommandWidget.h \
    Library/DeviceFilterWidget.h \
    OscTimeWidget.h \
    Action/ActionWidget.h \
    Rundown/RundownTreeBaseWidget.h \
    Library/DataTreeBaseWidget.h \
    Library/ToolTreeBaseWidget.h \
    Library/AudioTreeBaseWidget.h \
    Library/ImageTreeBaseWidget.h \
    Library/TemplateTreeBaseWidget.h \
    Library/VideoTreeBaseWidget.h \
    Library/PresetTreeBaseWidget.h \
    Rundown/RundownChromaWidget.h \
    Inspector/InspectorChromaWidget.h \
    Inspector/TemplateDataTreeBaseWidget.h \
    Inspector/TargetComboBaseWidget.h \
    Inspector/TriCaster/InspectorTakeWidget.h \
    Rundown/TriCaster/RundownTakeWidget.h \
    Inspector/TriCaster/InspectorAutoWidget.h \
    Rundown/TriCaster/RundownAutoWidget.h \
    Inspector/TriCaster/InspectorPresetWidget.h \
    Rundown/TriCaster/RundownPresetWidget.h \
    Inspector/TriCaster/InspectorInputWidget.h \
    Rundown/TriCaster/RundownInputWidget.h \
    TriCasterDeviceDialog.h \
    Inspector/TemplateDataDialog.h \
    ImportDeviceDialog.h \
    ImportTriCasterDeviceDialog.h \
    Rundown/TriCaster/RundownNetworkSourceWidget.h \
    Inspector/TriCaster/InspectorNetworkSourceWidget.h \
    Rundown/TriCaster/RundownMacroWidget.h \
    Inspector/TriCaster/InspectorMacroWidget.h \
    OscOutputDialog.h \
    Rundown/RundownOscOutputWidget.h \
    Inspector/InspectorOscOutputWidget.h \
    ImportAtemDeviceDialog.h \
    AtemDeviceDialog.h \
    Rundown/Atem/RundownAtemInputWidget.h \
    Rundown/Atem/RundownAtemAutoWidget.h \
    Inspector/Atem/InspectorAtemInputWidget.h \
    Rundown/Atem/RundownAtemCutWidget.h \
    Inspector/Atem/InspectorAtemCutWidget.h \
    Inspector/Atem/InspectorAtemAutoWidget.h \
    Rundown/Atem/RundownAtemKeyerStateWidget.h \
    Inspector/Atem/InspectorAtemKeyerStateWidget.h \
    Rundown/Atem/RundownAtemVideoFormatWidget.h \
    Inspector/Atem/InspectorAtemVideoFormatWidget.h \
    Inspector/Atem/InspectorAtemAudioInputStateWidget.h \
    Rundown/Atem/RundownAtemAudioInputStateWidget.h \
    Rundown/Atem/RundownAtemAudioInputBalanceWidget.h \
    Rundown/Atem/RundownAtemAudioGainWidget.h \
    Inspector/Atem/InspectorAtemAudioInputBalanceWidget.h \
    Inspector/Atem/InspectorAtemAudioGainWidget.h \
    Rundown/OpenRundownFromUrlDialog.h \
    AudioMeterWidget.h \
    AudioLevelsWidget.h \
    LiveWidget.h \
    Rundown/RundownPlayoutCommandWidget.h \
    Inspector/InspectorPlayoutCommandWidget.h

SOURCES += \
    SettingsDialog.cpp \
    PreviewWidget.cpp \
    AboutDialog.cpp \
    MainWindow.cpp \
    HelpDialog.cpp \
    Inspector/InspectorWidget.cpp \
    Inspector/InspectorTemplateWidget.cpp \
    Inspector/InspectorOutputWidget.cpp \
    Inspector/InspectorMetadataWidget.cpp \
    Inspector/InspectorLevelsWidget.cpp \
    Inspector/InspectorGeometryWidget.cpp \
    Inspector/InspectorImageScrollerWidget.cpp \
    Inspector/InspectorCropWidget.cpp \
    Inspector/InspectorBrightnessWidget.cpp \
    Inspector/InspectorOpacityWidget.cpp \
    Inspector/InspectorSaturationWidget.cpp \
    Inspector/InspectorContrastWidget.cpp \
    Inspector/InspectorVolumeWidget.cpp \
    Inspector/InspectorGridWidget.cpp \
    Inspector/InspectorGpiOutputWidget.cpp \
    Inspector/InspectorFileRecorderWidget.cpp \
    Library/LibraryWidget.cpp \
    Rundown/RundownCropWidget.cpp \
    Rundown/RundownGroupWidget.cpp \
    Rundown/RundownTemplateWidget.cpp \
    Rundown/RundownWidget.cpp \
    Rundown/RundownGeometryWidget.cpp \
    Rundown/RundownBrightnessWidget.cpp \
    Rundown/RundownSaturationWidget.cpp \
    Rundown/RundownOpacityWidget.cpp \
    Rundown/RundownContrastWidget.cpp \
    Rundown/RundownVolumeWidget.cpp \
    Rundown/RundownLevelsWidget.cpp \
    Rundown/RundownKeyerWidget.cpp \
    Rundown/RundownGridWidget.cpp \
    Rundown/RundownGpiOutputWidget.cpp \
    Rundown/RundownCommitWidget.cpp \
    Rundown/RundownImageScrollerWidget.cpp \
    Rundown/RundownFileRecorderWidget.cpp \
    Inspector/InspectorBlendModeWidget.cpp \
    Rundown/RundownBlendModeWidget.cpp \
    Rundown/RundownSeparatorWidget.cpp \
    Rundown/RundownPrintWidget.cpp \
    Inspector/InspectorKeyerWidget.cpp \
    Inspector/InspectorPrintWidget.cpp \
    Rundown/RundownClearOutputWidget.cpp \
    Inspector/InspectorClearOutputWidget.cpp \
    Inspector/InspectorGroupWidget.cpp \
    Rundown/RundownAudioWidget.cpp \
    Inspector/InspectorAudioWidget.cpp \
    Inspector/InspectorDeckLinkInputWidget.cpp \
    Rundown/RundownDeckLinkInputWidget.cpp \
    Rundown/RundownSolidColorWidget.cpp \
    Inspector/InspectorSolidColorWidget.cpp \
    Rundown/RundownImageWidget.cpp \
    Inspector/InspectorImageWidget.cpp \
    Rundown/RundownVideoWidget.cpp \
    Inspector/InspectorVideoWidget.cpp \
    Rundown/RundownTreeWidget.cpp \
    Rundown/RundownItemFactory.cpp \
    DeviceDialog.cpp \
    PresetDialog.cpp \
    Rundown/RundownCustomCommandWidget.cpp \
    Inspector/InspectorCustomCommandWidget.cpp \
    Library/DeviceFilterWidget.cpp \
    OscTimeWidget.cpp \
    Action/ActionWidget.cpp \
    Rundown/RundownTreeBaseWidget.cpp \
    Library/DataTreeBaseWidget.cpp \
    Library/ToolTreeBaseWidget.cpp \
    Library/AudioTreeBaseWidget.cpp \
    Library/ImageTreeBaseWidget.cpp \
    Library/TemplateTreeBaseWidget.cpp \
    Library/VideoTreeBaseWidget.cpp \
    Library/PresetTreeBaseWidget.cpp \
    Rundown/RundownChromaWidget.cpp \
    Inspector/InspectorChromaWidget.cpp \
    Inspector/TemplateDataTreeBaseWidget.cpp \
    Inspector/TargetComboBaseWidget.cpp \
    Inspector/TriCaster/InspectorTakeWidget.cpp \
    Rundown/TriCaster/RundownTakeWidget.cpp \
    Inspector/TriCaster/InspectorAutoWidget.cpp \
    Rundown/TriCaster/RundownAutoWidget.cpp \
    Inspector/TriCaster/InspectorPresetWidget.cpp \
    Rundown/TriCaster/RundownPresetWidget.cpp \
    Inspector/TriCaster/InspectorInputWidget.cpp \
    Rundown/TriCaster/RundownInputWidget.cpp \
    TriCasterDeviceDialog.cpp \
    Inspector/TemplateDataDialog.cpp \
    ImportDeviceDialog.cpp \
    ImportTriCasterDeviceDialog.cpp \
    Rundown/TriCaster/RundownNetworkSourceWidget.cpp \
    Inspector/TriCaster/InspectorNetworkSourceWidget.cpp \
    Rundown/TriCaster/RundownMacroWidget.cpp \
    Inspector/TriCaster/InspectorMacroWidget.cpp \
    OscOutputDialog.cpp \
    Rundown/RundownOscOutputWidget.cpp \
    Inspector/InspectorOscOutputWidget.cpp \
    ImportAtemDeviceDialog.cpp \
    AtemDeviceDialog.cpp \
    Rundown/Atem/RundownAtemInputWidget.cpp \
    Rundown/Atem/RundownAtemAutoWidget.cpp \
    Inspector/Atem/InspectorAtemInputWidget.cpp \
    Rundown/Atem/RundownAtemCutWidget.cpp \
    Inspector/Atem/InspectorAtemCutWidget.cpp \
    Inspector/Atem/InspectorAtemAutoWidget.cpp \
    Rundown/Atem/RundownAtemKeyerStateWidget.cpp \
    Inspector/Atem/InspectorAtemKeyerStateWidget.cpp \
    Rundown/Atem/RundownAtemVideoFormatWidget.cpp \
    Inspector/Atem/InspectorAtemVideoFormatWidget.cpp \
    Inspector/Atem/InspectorAtemAudioInputStateWidget.cpp \
    Rundown/Atem/RundownAtemAudioInputStateWidget.cpp \
    Rundown/Atem/RundownAtemAudioInputBalanceWidget.cpp \
    Rundown/Atem/RundownAtemAudioGainWidget.cpp \
    Inspector/Atem/InspectorAtemAudioInputBalanceWidget.cpp \
    Inspector/Atem/InspectorAtemAudioGainWidget.cpp \
    Rundown/OpenRundownFromUrlDialog.cpp \
    AudioMeterWidget.cpp \
    AudioLevelsWidget.cpp \
    LiveWidget.cpp \
    Rundown/RundownPlayoutCommandWidget.cpp \
    Inspector/InspectorPlayoutCommandWidget.cpp

FORMS += \
    SettingsDialog.ui \
    MainWindow.ui \
    PreviewWidget.ui \
    AboutDialog.ui \
    HelpDialog.ui \
    Inspector/InspectorVolumeWidget.ui \
    Inspector/InspectorWidget.ui \
    Inspector/InspectorTemplateWidget.ui \
    Inspector/InspectorOutputWidget.ui \
    Inspector/InspectorMetadataWidget.ui \
    Inspector/InspectorGeometryWidget.ui \
    Inspector/InspectorGridWidget.ui \
    Inspector/InspectorDeckLinkInputWidget.ui \
    Inspector/InspectorGpiOutputWidget.ui \
    Inspector/InspectorImageScrollerWidget.ui \
    Inspector/InspectorFileRecorderWidget.ui \
    Library/LibraryWidget.ui \
    Rundown/RundownWidget.ui \
    Rundown/RundownTemplateWidget.ui \
    Rundown/RundownGroupWidget.ui \
    Inspector/InspectorLevelsWidget.ui \
    Inspector/InspectorCropWidget.ui \
    Rundown/RundownCropWidget.ui \
    Inspector/InspectorBrightnessWidget.ui \
    Rundown/RundownGeometryWidget.ui \
    Rundown/RundownBrightnessWidget.ui \
    Inspector/InspectorSaturationWidget.ui \
    Rundown/RundownSaturationWidget.ui \
    Rundown/RundownOpacityWidget.ui \
    Inspector/InspectorOpacityWidget.ui \
    Rundown/RundownContrastWidget.ui \
    Inspector/InspectorContrastWidget.ui \
    Rundown/RundownVolumeWidget.ui \
    Rundown/RundownLevelsWidget.ui \
    Rundown/RundownKeyerWidget.ui \
    Rundown/RundownGridWidget.ui \ 
    Rundown/RundownGpiOutputWidget.ui \
    Rundown/RundownCommitWidget.ui \
    Rundown/RundownImageScrollerWidget.ui \ 
    Rundown/RundownFileRecorderWidget.ui \
    Inspector/InspectorBlendModeWidget.ui \
    Rundown/RundownBlendModeWidget.ui \
    Rundown/RundownSeparatorWidget.ui \
    Rundown/RundownPrintWidget.ui \
    Inspector/InspectorKeyerWidget.ui \
    Inspector/InspectorPrintWidget.ui \
    Rundown/RundownClearOutputWidget.ui \
    Inspector/InspectorClearOutputWidget.ui \
    Inspector/InspectorGroupWidget.ui \
    Rundown/RundownAudioWidget.ui \
    Inspector/InspectorAudioWidget.ui \
    Rundown/RundownDeckLinkInputWidget.ui \
    Rundown/RundownSolidColorWidget.ui \
    Inspector/InspectorSolidColorWidget.ui \
    Rundown/RundownImageWidget.ui \
    Inspector/InspectorImageWidget.ui \
    Rundown/RundownVideoWidget.ui \
    Inspector/InspectorVideoWidget.ui \
    Rundown/RundownTreeWidget.ui \
    DeviceDialog.ui \
    PresetDialog.ui \
    Rundown/RundownCustomCommandWidget.ui \
    Inspector/InspectorCustomCommandWidget.ui \
    Library/DeviceFilterWidget.ui \
    OscTimeWidget.ui \
    Action/ActionWidget.ui \
    Rundown/RundownChromaWidget.ui \
    Inspector/InspectorChromaWidget.ui \
    Inspector/TriCaster/InspectorTakeWidget.ui \
    Rundown/TriCaster/RundownTakeWidget.ui \
    Inspector/TriCaster/InspectorAutoWidget.ui \
    Rundown/TriCaster/RundownAutoWidget.ui \
    Inspector/TriCaster/InspectorPresetWidget.ui \
    Rundown/TriCaster/RundownPresetWidget.ui \
    Inspector/TriCaster/InspectorInputWidget.ui \
    Rundown/TriCaster/RundownInputWidget.ui \
    TriCasterDeviceDialog.ui \
    Inspector/TemplateDataDialog.ui \
    ImportDeviceDialog.ui \
    ImportTriCasterDeviceDialog.ui \
    Rundown/TriCaster/RundownNetworkSourceWidget.ui \
    Inspector/TriCaster/InspectorNetworkSourceWidget.ui \
    Rundown/TriCaster/RundownMacroWidget.ui \
    Inspector/TriCaster/InspectorMacroWidget.ui \
    OscOutputDialog.ui \
    Rundown/RundownOscOutputWidget.ui \
    Inspector/InspectorOscOutputWidget.ui \
    ImportAtemDeviceDialog.ui \
    AtemDeviceDialog.ui \
    Rundown/Atem/RundownAtemInputWidget.ui \
    Rundown/Atem/RundownAtemAutoWidget.ui \
    Inspector/Atem/InspectorAtemInputWidget.ui \
    Rundown/Atem/RundownAtemCutWidget.ui \
    Inspector/Atem/InspectorAtemCutWidget.ui \
    Inspector/Atem/InspectorAtemAutoWidget.ui \
    Rundown/Atem/RundownAtemKeyerStateWidget.ui \
    Inspector/Atem/InspectorAtemKeyerStateWidget.ui \
    Rundown/Atem/RundownAtemVideoFormatWidget.ui \
    Inspector/Atem/InspectorAtemVideoFormatWidget.ui \
    Inspector/Atem/InspectorAtemAudioInputStateWidget.ui \
    Rundown/Atem/RundownAtemAudioInputStateWidget.ui \
    Rundown/Atem/RundownAtemAudioInputBalanceWidget.ui \
    Rundown/Atem/RundownAtemAudioGainWidget.ui \
    Inspector/Atem/InspectorAtemAudioInputBalanceWidget.ui \
    Inspector/Atem/InspectorAtemAudioGainWidget.ui \
    Rundown/OpenRundownFromUrlDialog.ui \
    AudioMeterWidget.ui \
    AudioLevelsWidget.ui \
    LiveWidget.ui \
    Rundown/RundownPlayoutCommandWidget.ui \
    Inspector/InspectorPlayoutCommandWidget.ui

RESOURCES += \
    Widgets.qrc \

OTHER_FILES += \
    Images/ArrowUpDisabled.png \
    Images/ArrowUp.png \
    Images/ArrowDownDisabled.png \
    Images/ArrowDown.png \
    Stylesheets/Default.css \
    Stylesheets/Extended.css \
    Images/RadiobuttonUncheckedPressed.png \
    Images/RadiobuttonUncheckedHover.png \
    Images/RadiobuttonUnchecked.png \
    Images/RadiobuttonCheckedPressed.png \
    Images/RadiobuttonCheckedHover.png \
    Images/RadiobuttonChecked.png \
    Images/CheckboxUncheckedPressed.png \
    Images/CheckboxUncheckedHover.png \
    Images/CheckboxUnchecked.png \
    Images/CheckboxCheckedPressed.png \
    Images/CheckboxCheckedHover.png \
    Images/Logo.png \
    Images/CasparCG.png \
    Images/ArrowLeftDisabled.png \
    Images/ArrowLeft.png \
    Images/ArrowRightDisabled.png \
    Images/ArrowRight.png \
    Images/Forward.png \
    Images/Rewind.png \
    Images/RewindStart.png \
    Images/CheckboxChecked.png \
    Images/Group.png \
    Images/Audio.png \
    Images/Mixer.png \
    Images/Template.png \
    Images/Preview.png \
    Images/Movie.png \
    Images/Still.png \
    Images/Disconnected.png \
    Images/New.png \
    Images/ImageScroller.png \
    Images/GpiDisconnected.png \
    Images/GpiConnected.png \
    Images/Print.png \
    Images/Clear.png \
    Images/AutoStep.png \
    Images/Snapshot.png \
    Images/MasterVolumeOff.png \
    Images/MasterVolumeOn.png \
    Images/ServerSmall.png \
    Images/ServerBig.png \
    Images/Thumbnail.png \
    Images/SplitterVertical.png \
    Images/SplitterHorizontal.png \
    Images/Checkerboard.png \
    Images/TestConnection.png \
    Images/Information.png \
    Images/Attention.png \
    Images/Dropdown.png \
    Images/Close.png \
    Images/CloseHover.png \
    Images/TabSplitter.png \
    Images/DropdownHover.png \
    Images/PreviewAlpha.png \
    Images/PreviewAlphaHover.png \
    Images/RemoveHover.png \
    Images/Remove.png \
    Images/AddHover.png \
    Images/Add.png \
    Images/AudioSmallHover.png \
    Images/AudioSmall.png \
    Images/StillSmallHover.png \
    Images/StillSmall.png \
    Images/ImageScrollerSmallHover.png \
    Images/ImageScrollerSmall.png \
    Images/TemplateSmallHover.png \
    Images/TemplateSmall.png \
    Images/MovieSmallHover.png \
    Images/MovieSmall.png \
    Images/VolumeSmallHover.png \
    Images/VolumeSmall.png \
    Images/SaturationSmallHover.png \
    Images/SaturationSmall.png \
    Images/OpacitySmallHover.png \
    Images/OpacitySmall.png \
    Images/LevelsSmallHover.png \
    Images/LevelsSmall.png \
    Images/KeyerSmallHover.png \
    Images/KeyerSmall.png \
    Images/GridSmallHover.png \
    Images/GridSmall.png \
    Images/DeckLinkProducerSmallHover.png \
    Images/DeckLinkProducerSmall.png \
    Images/CropSmallHover.png \
    Images/CropSmall.png \
    Images/ContrastSmallHover.png \
    Images/ContrastSmall.png \
    Images/CommitSmallHover.png \
    Images/CommitSmall.png \
    Images/ClearSmallHover.png \
    Images/ClearSmall.png \
    Images/BrightnessSmallHover.png \
    Images/BrightnessSmall.png \
    Images/DeckLinkProducer.png \
    Images/SeparatorSmallHover.png \
    Images/SeparatorSmall.png \
    Images/Snapshot.png \
    Images/SnapshotSmallHover.png \
    Images/SnapshotSmall.png \
    Images/GeometrySmallHover.png \
    Images/GeometrySmall.png \
    Images/FileRecorder.png \
    Images/FileRecorderSmallHover.png \
    Images/FileRecorderSmall.png \
    Images/GroupSmallHover.png \
    Images/GroupSmall.png \
    Images/UngroupSmallHover.png \
    Images/UngroupSmall.png \
    Images/GpiOutputSmallHover.png \
    Images/GpiOutputSmall.png \
    Images/SolidColorSmallHover.png \
    Images/SolidColorSmall.png \
    Images/Opacity.png \
    Images/Contrast.png \
    Images/Brightness.png \
    Images/BlendModeSmallHover.png \
    Images/BlendModeSmall.png \
    Images/Grid.png \
    Images/Geometry.png \
    Images/Crop.png \
    Images/SolidColorSmallHover.png \
    Images/SolidColorSmall.png \
    Images/SolidColor.png \
    Images/PresetSmall.png \
    Images/DataSmall.png \
    Images/Preset.png \
    Images/Saturation.png \
    Images/CustomCommand.png \
    Images/PresetSmallHover.png \
    Images/CustomCommandSmallHover.png \
    Images/CustomCommandSmall.png \
    Images/Commit.png \
    Images/ToolbarSplitterHorizontal.png \
    Images/ToolbarSplitterVertical.png \
    Stylesheets/Linux.css \
    Stylesheets/Mac.css \
    Images/LoopIconOverlay.png \
    Images/PlayProgressBarOverlay.png \
    Images/PauseProgressBarOverlay.png \
    Images/LoopProgressBarOverlay.png \
    Images/ChromaSmall.png \
    Images/Chroma.png \
    Images/AutoPlayItem.png \
    Images/AutoPlayGroup.png \
    Images/MixerSmall.png \
    Images/DisableCommandOn.png \
    Images/DisableCommandOff.png \
    Images/Import.png \
    Images/ImportHover.png \
    Images/OscOutputSmallHover.png \
    Images/OscOutputSmall.png \
    Images/OscOutput.png \
    Images/TriCaster/TriggerTakeSmallHover.png \
    Images/TriCaster/TriggerTakeSmall.png \
    Images/TriCaster/TriggerTake.png \
    Images/TriCaster/TriggerAutoSmallHover.png \
    Images/TriCaster/TriggerAutoSmall.png \
    Images/TriCaster/TriggerAuto.png \
    Images/TriCaster/SelectPresetSmallHover.png \
    Images/TriCaster/SelectPresetSmall.png \
    Images/TriCaster/SelectPreset.png \
    Images/TriCaster/SelectNetworkSourceSmallHover.png \
    Images/TriCaster/SelectNetworkSourceSmall.png \
    Images/TriCaster/SelectNetworkSource.png \
    Images/TriCaster/SelectInputSmallHover.png \
    Images/TriCaster/SelectInputSmall.png \
    Images/TriCaster/SelectInput.png \
    Images/TriCaster/PlayMacroSmallHover.png \
    Images/TriCaster/PlayMacroSmall.png \
    Images/TriCaster/PlayMacro.png \
    Images/Clients.png \
    Stylesheets/Curve/Windows.css \
    Stylesheets/Curve/Mac.css \
    Stylesheets/Curve/Linux.css \
    Stylesheets/Curve/Extended.css \
    Stylesheets/Curve/Default.css \
    Stylesheets/Flat/Windows.css \
    Stylesheets/Flat/Mac.css \
    Stylesheets/Flat/Linux.css \
    Stylesheets/Flat/Extended.css \
    Stylesheets/Flat/Default.css \
    Images/OscTriggerSmall.png \
    Images/Atem/SelectInputSmallHover.png \
    Images/Atem/SelectInputSmall.png \
    Images/Atem/SelectInput.png \
    Images/Atem/TriggerCutSmallHover.png \
    Images/Atem/TriggerCutSmall.png \
    Images/Atem/TriggerCut.png \
    Images/Atem/TriggerAutoSmallHover.png \
    Images/Atem/TriggerAutoSmall.png \
    Images/Atem/TriggerAuto.png \
    Images/AtemTriggerCutSmallHover.png \
    Images/AtemTriggerCutSmall.png \
    Images/AtemTriggerCut.png \
    Images/AtemAudioGainSmall.png \
    Images/Atem/AudioGainSmall.png \
    Images/Atem/AudioGain.png \
    Images/Atem/AudioGainSmallHover.png \
    Images/Atem/VideoFormatSmallHover.png \
    Images/Atem/VideoFormatSmall.png \
    Images/Atem/VideoFormat.png \
    Images/Atem/KeyerStateSmallHover.png \
    Images/Atem/KeyerStateSmall.png \
    Images/Atem/KeyerState.png \
    Images/Atem/AudioInputStateSmallHover.png \
    Images/Atem/AudioInputStateSmall.png \
    Images/Atem/AudioInputState.png \
    Images/Atem/AudioInputBalence.png \
    Images/Atem/AudioInputBalanceSmallHover.png \
    Images/Atem/AudioInputBalanceSmall.png \
    Images/Rundown.png \
    Images/Sienna.png \
    Images/OliveDrab.png \
    Images/SeaGreen.png \
    Images/Chocolate.png \
    Images/DarkSlateGray.png \
    Images/SteelBlue.png \
    Images/Maroon.png \
    Images/MaroonLight.png \
    Images/DarkKhaki.png \
    Images/RoyalBlue.png \
    Images/StreamNotAvailable.png

DEPENDPATH += $$PWD/../../lib/qatemcontrol/include
INCLUDEPATH += $$PWD/../../lib/qatemcontrol/include
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/win32/release/ -lqatemcontrol
else:win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/win32/debug/ -lqatemcontrol
else:macx:CONFIG(release, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/macx/release/ -lqatemcontrol
else:macx:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/macx/debug/ -lqatemcontrol
else:unix:CONFIG(release, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/linux/release/ -lqatemcontrol
else:unix:CONFIG(debug, debug|release):LIBS += -L$$PWD/../../lib/qatemcontrol/lib/linux/debug/ -lqatemcontrol

DEPENDPATH += $$PWD/../../lib/oscpack/include
INCLUDEPATH += $$PWD/../../lib/oscpack/include
win32:LIBS += -L$$PWD/../../lib/oscpack/lib/win32/ -loscpack
else:macx:LIBS += -L$$PWD/../../lib/oscpack/lib/macx/ -loscpack
else:unix:LIBS += -L$$PWD/../../lib/oscpack/lib/linux/ -loscpack

DEPENDPATH += $$PWD/../../lib/boost
INCLUDEPATH += $$PWD/../../lib/boost
win32:LIBS += -L$$PWD/../../lib/boost/stage/lib/win32/ -lboost_date_time-mgw44-mt-1_47 -lboost_system-mgw44-mt-1_47 -lboost_thread-mgw44-mt-1_47 -lboost_filesystem-mgw44-mt-1_47 -lboost_chrono-mgw44-mt-1_47 -lws2_32
else:macx:LIBS += -L$$PWD/../../lib/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono
else:unix:LIBS += -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono

DEPENDPATH += $$PWD/../../lib/libvlc/include
INCLUDEPATH += $$PWD/../../lib/libvlc/include
win32:LIBS += -L$$PWD/../../lib/libvlc/lib/win32/ -lvlc -lvlccore
else:macx:LIBS += -L$$PWD/../../lib/libvlc/lib/macx/ -lvlc -lvlccore
else:unix:LIBS += -L$$PWD/../../lib/libvlc/lib/linux/ -lvlc -lvlccore

DEPENDPATH += $$PWD/../../lib/gpio-client/include
INCLUDEPATH += $$PWD/../../lib/gpio-client/include
win32:LIBS += -L$$PWD/../../lib/gpio-client/lib/win32/ -lgpio-client
else:macx:LIBS += -L$$PWD/../../lib/gpio-client/lib/macx/ -lgpio-client
else:unix:LIBS += -L$$PWD/../../lib/gpio-client/lib/linux/ -lgpio-client

DEPENDPATH += $$OUT_PWD/../Atem $$PWD/../Atem
INCLUDEPATH += $$OUT_PWD/../Atem $$PWD/../Atem
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Atem/release/ -latem
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Atem/debug/ -latem
else:macx:LIBS += -L$$OUT_PWD/../Atem/ -latem
else:unix:LIBS += -L$$OUT_PWD/../Atem/ -latem

DEPENDPATH += $$OUT_PWD/../Caspar $$PWD/../Caspar
INCLUDEPATH += $$OUT_PWD/../Caspar $$PWD/../Caspar
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Caspar/release/ -lcaspar
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Caspar/debug/ -lcaspar
else:macx:LIBS += -L$$OUT_PWD/../Caspar/ -lcaspar
else:unix:LIBS += -L$$OUT_PWD/../Caspar/ -lcaspar

DEPENDPATH += $$OUT_PWD/../TriCaster $$PWD/../TriCaster
INCLUDEPATH += $$OUT_PWD/../TriCaster $$PWD/../TriCaster
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../TriCaster/release/ -ltricaster
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../TriCaster/debug/ -ltricaster
else:macx:LIBS += -L$$OUT_PWD/../TriCaster/ -ltricaster
else:unix:LIBS += -L$$OUT_PWD/../TriCaster/ -ltricaster

DEPENDPATH += $$OUT_PWD/../Osc $$PWD/../Osc
INCLUDEPATH += $$OUT_PWD/../Osc $$PWD/../Osc
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Osc/release/ -losc
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Osc/debug/ -losc
else:macx:LIBS += -L$$OUT_PWD/../Osc/ -losc
else:unix:LIBS += -L$$OUT_PWD/../Osc/ -losc

DEPENDPATH += $$OUT_PWD/../Gpi $$PWD/../Gpi
INCLUDEPATH += $$OUT_PWD/../Gpi $$PWD/../Gpi
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Gpi/release/ -lgpi
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Gpi/debug/ -lgpi
else:macx:LIBS += -L$$OUT_PWD/../Gpi/ -lgpi
else:unix:LIBS += -L$$OUT_PWD/../Gpi/ -lgpi

DEPENDPATH += $$OUT_PWD/../Common $$PWD/../Common
INCLUDEPATH += $$OUT_PWD/../Common $$PWD/../Common
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Common/release/ -lcommon
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Common/debug/ -lcommon
else:macx:LIBS += -L$$OUT_PWD/../Common/ -lcommon
else:unix:LIBS += -L$$OUT_PWD/../Common/ -lcommon

DEPENDPATH += $$OUT_PWD/../Core $$PWD/../Core
INCLUDEPATH += $$OUT_PWD/../Core $$PWD/../Core
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Core/release/ -lcore
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Core/debug/ -lcore
else:macx:LIBS += -L$$OUT_PWD/../Core/ -lcore
else:unix:LIBS += -L$$OUT_PWD/../Core/ -lcore
