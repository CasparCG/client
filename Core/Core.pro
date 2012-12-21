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
    MediaPlayer.h \
    Shared.h \
    Commands/CropCommand.h \
    Commands/GeometryCommand.h \
    Commands/MediaCommand.h \
    Commands/TemplateCommand.h \
    Events/AddRudnownItemEvent.h \
    Events/AutoSynchronizeEvent.h \
    Events/DataChangedEvent.h \
    Events/MediaChangedEvent.h \
    Events/RundownItemPreviewEvent.h \
    Events/RundownItemSelectedEvent.h \
    Events/StatusbarEvent.h \
    Events/SynchronizeEvent.h \
    Events/TemplateChangedEvent.h \
    Models/ConfigurationModel.h \
    Models/DeviceModel.h \
    Models/LibraryModel.h \
    Models/RundownModel.h \
    Models/TemplateDataModel.h \
    Models/TypeModel.h \
    Events/LibraryItemSelectedEvent.h \
    Events/RundownItemChangedEvent.h \
    Commands/BrightnessCommand.h \
    Commands/SaturationCommand.h \
    Commands/OpacityCommand.h \
    Commands/ContrastCommand.h \
    Commands/VolumeCommand.h \
    Commands/LevelsCommand.h \
    Commands/KeyerCommand.h \
    Commands/GridCommand.h \
    Events/ConnectionStateChangedEvent.h \
    Commands/ICommand.h \
    Commands/GpiOutputCommand.h \
    Events/RundownIsEmptyEvent.h \
    SynchronizeManager.h \
    GpiManager.h \
    Models/GpiModel.h \
    Commands/DeckLinkInputCommand.h \
    Models/FormatModel.h \
    Models/TransitionModel.h \
    Models/DirectionModel.h \
    Models/TweenModel.h \
    Models/GpiDeviceModel.h \
    Models/GpiPortModel.h \
    Models/GpoPortModel.h \
    Commands/CommitCommand.h \
    Commands/ImageScrollerCommand.h \
    Commands/FileRecorderCommand.h \
    Commands/GroupCommand.h \
    Events/OpenRundownEvent.h \
    Commands/BlendModeCommand.h \
    Models/BlendModeModel.h \
    Events/SaveRundownEvent.h

SOURCES += \
    DatabaseManager.cpp \
    DeviceManager.cpp \
    MediaPlayer.cpp \
    Commands/CropCommand.cpp \
    Commands/GeometryCommand.cpp \
    Commands/MediaCommand.cpp \
    Commands/TemplateCommand.cpp \
    Events/AddRudnownItemEvent.cpp \
    Events/AutoSynchronizeEvent.cpp \
    Events/DataChangedEvent.cpp \
    Events/MediaChangedEvent.cpp \
    Events/RundownItemPreviewEvent.cpp\
    Events/RundownItemSelectedEvent.cpp \
    Events/StatusbarEvent.cpp \
    Events/SynchronizeEvent.cpp \
    Events/TemplateChangedEvent.cpp \
    Models/ConfigurationModel.cpp \
    Models/DeviceModel.cpp \
    Models/LibraryModel.cpp \
    Models/RundownModel.cpp \
    Models/TypeModel.cpp \
    Events/LibraryItemSelectedEvent.cpp \
    Models/TemplateDataModel.cpp \
    Events/RundownItemChangedEvent.cpp \
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
    Events/RundownIsEmptyEvent.cpp \
    SynchronizeManager.cpp \
    GpiManager.cpp \
    Models/GpiModel.cpp \
    Commands/DeckLinkInputCommand.cpp \
    Models/FormatModel.cpp \
    Models/TransitionModel.cpp \
    Models/DirectionModel.cpp \
    Models/TweenModel.cpp \
    Models/GpiDeviceModel.cpp \
    Models/GpiPortModel.cpp \
    Models/GpoPortModel.cpp \
    Commands/CommitCommand.cpp \
    Commands/ImageScrollerCommand.cpp \
    Commands/FileRecorderCommand.cpp \
    Commands/GroupCommand.cpp \
    Events/OpenRundownEvent.cpp \
    Models/BlendModeModel.cpp \
    Commands/BlendModeCommand.cpp \
    Events/SaveRundownEvent.cpp

INCLUDEPATH += $$PWD/../../dependencies/boost
win32: LIBS += -L$$PWD/../../dependencies/boost/stage/lib/win32/ -lboost_date_time-mgw44-mt-1_47 -lboost_system-mgw44-mt-1_47 -lboost_thread-mgw44-mt-1_47 -lboost_filesystem-mgw44-mt-1_47 -lboost_chrono-mgw44-mt-1_47 -lws2_32
else:macx: LIBS += -L$$PWD/../../dependencies/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono

DEPENDPATH += $$PWD/../../dependencies/gpio-client/include
INCLUDEPATH += $$PWD/../../dependencies/gpio-client/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/gpio-client/lib/win32/release/ -lgpio-client
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/gpio-client/lib/win32/debug/ -lgpio-client
else:macx: LIBS += -L$$PWD/../../dependencies/gpio-client/lib/macx/ -lgpio-client

DEPENDPATH += $$PWD/../Caspar $$OUT_PWD/../Caspar
INCLUDEPATH += $$PWD/../Caspar $$OUT_PWD/../Caspar
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Caspar/release/ -lcaspar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Caspar/debug/ -lcaspar
else:unix: LIBS += -L$$OUT_PWD/../Caspar/ -lcaspar

DEPENDPATH += $$PWD/../Gpi $$OUT_PWD/../Gpi
INCLUDEPATH += $$PWD/../Gpi $$OUT_PWD/../Gpi
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Gpi/release/ -lgpi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Gpi/debug/ -lgpi
else:unix: LIBS += -L$$OUT_PWD/../Gpi/ -lGpi

DEPENDPATH += $$PWD/../Common $$OUT_PWD/../Common
INCLUDEPATH += $$PWD/../Common $$OUT_PWD/../Common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../Common/ -lcommon
