#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:26:10
#
#-------------------------------------------------

QT += core

TARGET = common
TEMPLATE = lib

DEFINES += COMMON_LIBRARY

HEADERS += \
    Global.h \
    Shared.h \
    Timecode.h
	
SOURCES += \
    Timecode.cpp

OTHER_FILES += \
    Version.h.in

GIT_TIMESTAMP = $$system($$quote(git log -n 1 --format=format:"%ai"))
GIT_VERSION = $$system($$quote(git rev-parse --verify --short=10 HEAD))

QMAKE_SUBSTITUTES += $$PWD/Version.h.in
