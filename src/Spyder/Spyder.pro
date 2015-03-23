QT -= gui
QT += network

CONFIG += c++11

TARGET = spyder
TEMPLATE = lib

DEFINES += SPYDER_LIBRARY

HEADERS += \
    Shared.h \
    SpyderDevice.h
	
SOURCES += \
    SpyderDevice.cpp
