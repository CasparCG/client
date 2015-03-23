QT -= gui
QT += network

CONFIG += c++11

TARGET = tricaster
TEMPLATE = lib

DEFINES += TRICASTER_LIBRARY

HEADERS += \
    TriCasterDevice.h \
    Shared.h \
    NtfcDevice.h
	
SOURCES += \
    TriCasterDevice.cpp \
    NtfcDevice.cpp
