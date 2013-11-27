#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:07:49
#
#-------------------------------------------------
QT -= gui
QT += network

TARGET = tricaster
TEMPLATE = lib

DEFINES += TRICASTER_LIBRARY

HEADERS += \
    TriCasterDevice.h \
    Shared.h \
    NTFCDevice.h
	
SOURCES += \
    TriCasterDevice.cpp \
    NTFCDevice.cpp
