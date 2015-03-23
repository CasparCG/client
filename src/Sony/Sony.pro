QT -= gui
QT += network

CONFIG += c++11

TARGET = sony
TEMPLATE = lib

DEFINES += SONY_LIBRARY

HEADERS += \
    Shared.h \
    ViscaDevice.h
	
SOURCES += \
    ViscaDevice.cpp
