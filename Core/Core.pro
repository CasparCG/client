#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:26:10
#
#-------------------------------------------------

QT += core sql

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

HEADERS += \
    DatabaseManager.h \
    DeviceManager.h \
    Shared.h \
    Commands/CropCommand.h \
    Commands/GeometryCommand.h \
    Commands/TemplateCommand.h \
    Events/AddRudnownItemEvent.h \
    Events/DataChangedEvent.h \
    Events/MediaChangedEvent.h \
    Events/RundownItemSelectedEvent.h \
    Events/StatusbarEvent.h \
    Events/TemplateChangedEvent.h \
    Models/ConfigurationModel.h \
    Models/DeviceModel.h \
    Models/LibraryModel.h \
    Models/RundownModel.h \
    Models/TemplateDataModel.h \
    Models/TypeModel.h \
    Events/LibraryItemSelectedEvent.h \
    Commands/BrightnessCommand.h \
    Commands/SaturationCommand.h \
    Commands/OpacityCommand.h \
    Commands/ContrastCommand.h \
    Commands/VolumeCommand.h \
    Commands/LevelsCommand.h \
    Commands/KeyerCommand.h \
    Commands/GridCommand.h \
    Events/ConnectionStateChangedEvent.h \
    Commands/GpiOutputCommand.h \
    GpiManager.h \
    Models/GpiModel.h \
    Commands/DeckLinkInputCommand.h \
    Models/FormatModel.h \
    Models/TransitionModel.h \
    Models/DirectionModel.h \
    Models/TweenModel.h \
    Models/GpiPortModel.h \
    Models/GpoPortModel.h \
    Commands/CommitCommand.h \
    Commands/ImageScrollerCommand.h \
    Commands/FileRecorderCommand.h \
    Commands/GroupCommand.h \
    Events/OpenRundownEvent.h \
    Commands/BlendModeCommand.h \
    Models/BlendModeModel.h \
    Events/SaveRundownEvent.h \
    LibraryManager.h \
    Events/RefreshLibraryEvent.h \
    Events/AutoRefreshLibraryEvent.h \
    Events/CompactViewEvent.h \
    Commands/SeparatorCommand.h \
    Commands/PrintCommand.h \
    Commands/AbstractCommand.h \
    Commands/AbstractProperties.h \
    Commands/AbstractPlayoutCommand.h \
    Commands/ClearOutputCommand.h \
    Animations/BorderAnimation.h \
    Animations/ActiveAnimation.h \
    Models/ThumbnailModel.h \
    ThumbnailWorker.h \
    Events/WindowTitleEvent.h \
    Commands/AudioCommand.h \
    Commands/SolidColorCommand.h \
    Commands/ImageCommand.h \
    Commands/VideoCommand.h \
    Events/DeviceChangedEvent.h \
    Events/LabelChangedEvent.h \
    Events/TargetChangedEvent.h \
    Events/PreviewEvent.h \
    EventManager.h \
    Events/EmptyRundownEvent.h \
    Events/RemoveRundownEvent.h \
    Events/AddRundownEvent.h

SOURCES += \
    DatabaseManager.cpp \
    DeviceManager.cpp \
    Commands/CropCommand.cpp \
    Commands/GeometryCommand.cpp \
    Commands/TemplateCommand.cpp \
    Events/AddRudnownItemEvent.cpp \
    Events/DataChangedEvent.cpp \
    Events/MediaChangedEvent.cpp \
    Events/RundownItemSelectedEvent.cpp \
    Events/StatusbarEvent.cpp \
    Events/TemplateChangedEvent.cpp \
    Models/ConfigurationModel.cpp \
    Models/DeviceModel.cpp \
    Models/LibraryModel.cpp \
    Models/RundownModel.cpp \
    Models/TypeModel.cpp \
    Events/LibraryItemSelectedEvent.cpp \
    Models/TemplateDataModel.cpp \
    Commands/BrightnessCommand.cpp \
    Commands/SaturationCommand.cpp \
    Commands/OpacityCommand.cpp \
    Commands/ContrastCommand.cpp \
    Commands/VolumeCommand.cpp \
    Commands/LevelsCommand.cpp \
    Commands/KeyerCommand.cpp \
    Commands/GridCommand.cpp \
    Events/ConnectionStateChangedEvent.cpp \
    Commands/GpiOutputCommand.cpp \
    GpiManager.cpp \
    Models/GpiModel.cpp \
    Commands/DeckLinkInputCommand.cpp \
    Models/FormatModel.cpp \
    Models/TransitionModel.cpp \
    Models/DirectionModel.cpp \
    Models/TweenModel.cpp \
    Models/GpiPortModel.cpp \
    Models/GpoPortModel.cpp \
    Commands/CommitCommand.cpp \
    Commands/ImageScrollerCommand.cpp \
    Commands/FileRecorderCommand.cpp \
    Commands/GroupCommand.cpp \
    Events/OpenRundownEvent.cpp \
    Models/BlendModeModel.cpp \
    Commands/BlendModeCommand.cpp \
    Events/SaveRundownEvent.cpp \
    LibraryManager.cpp \
    Events/AutoRefreshLibraryEvent.cpp \
    Events/RefreshLibraryEvent.cpp \
    Events/CompactViewEvent.cpp \
    Commands/SeparatorCommand.cpp \
    Commands/PrintCommand.cpp \
    Commands/AbstractCommand.cpp \
    Commands/ClearOutputCommand.cpp \
    Commands/AbstractPlayoutCommand.cpp \
    Commands/AbstractProperties.cpp \
    Animations/BorderAnimation.cpp \
    Animations/ActiveAnimation.cpp \
    Models/ThumbnailModel.cpp \
    ThumbnailWorker.cpp \
    Events/WindowTitleEvent.cpp \
    Commands/AudioCommand.cpp \
    Commands/SolidColorCommand.cpp \
    Commands/ImageCommand.cpp \
    Commands/VideoCommand.cpp \
    Events/DeviceChangedEvent.cpp \
    Events/LabelChangedEvent.cpp \
    Events/TargetChangedEvent.cpp \
    Events/PreviewEvent.cpp \
    EventManager.cpp \
    Events/EmptyRundownEvent.cpp \
    Events/RemoveRundownEvent.cpp \
    Events/AddRundownEvent.cpp

INCLUDEPATH += $$PWD/../Dependencies/boost
win32: LIBS += -L$$PWD/../Dependencies/boost/stage/lib/win32/ -lboost_date_time-mgw44-mt-1_47 -lboost_system-mgw44-mt-1_47 -lboost_thread-mgw44-mt-1_47 -lboost_filesystem-mgw44-mt-1_47 -lboost_chrono-mgw44-mt-1_47 -lws2_32
else:macx: LIBS += -L$$PWD/../Dependencies/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono

DEPENDPATH += $$PWD/../Dependencies/gpio-client/include
INCLUDEPATH += $$PWD/../Dependencies/gpio-client/include
win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../Dependencies/gpio-client/lib/win32/release/ -lgpio-client
else:win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/../Dependencies/gpio-client/lib/win32/debug/ -lgpio-client
else:macx:LIBS += -L$$PWD/../Dependencies/gpio-client/lib/macx/ -lgpio-client

DEPENDPATH += $$PWD/../Caspar $$OUT_PWD/../Caspar
INCLUDEPATH += $$PWD/../Caspar $$OUT_PWD/../Caspar
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Caspar/release/ -lcaspar
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Caspar/debug/ -lcaspar
else:unix:LIBS += -L$$OUT_PWD/../Caspar/ -lcaspar

DEPENDPATH += $$PWD/../Gpi $$OUT_PWD/../Gpi
INCLUDEPATH += $$PWD/../Gpi $$OUT_PWD/../Gpi
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Gpi/release/ -lgpi
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Gpi/debug/ -lgpi
else:unix:LIBS += -L$$OUT_PWD/../Gpi/ -lGpi

DEPENDPATH += $$PWD/../Common $$OUT_PWD/../Common
INCLUDEPATH += $$PWD/../Common $$OUT_PWD/../Common
win32:CONFIG(release, debug|release):LIBS += -L$$OUT_PWD/../Common/release/ -lcommon
else:win32:CONFIG(debug, debug|release):LIBS += -L$$OUT_PWD/../Common/debug/ -lcommon
else:unix:LIBS += -L$$OUT_PWD/../Common/ -lcommon
