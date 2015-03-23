QT += core

CONFIG += c++11

TARGET = common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

HEADERS += \
    Global.h \
    Shared.h \
    Timecode.h \
    Xml.h \
    Playout.h
	
SOURCES += \
    Timecode.cpp \
    Xml.cpp \
    Playout.cpp

OTHER_FILES += \
    Version.h.in

GIT_TIMESTAMP = $$system($$quote(git log -n 1 --format=format:"%ai"))
GIT_VERSION = $$system($$quote(git rev-parse --verify --short=10 HEAD))

QMAKE_SUBSTITUTES += $$PWD/Version.h.in
