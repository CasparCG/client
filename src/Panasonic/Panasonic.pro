QT -= gui
QT += network

CONFIG += c++11

TARGET = panasonic
TEMPLATE = lib

DEFINES += PANASONIC_LIBRARY

HEADERS += \
    Shared.h \
    PtzDevice.h
	
SOURCES += \
    PtzDevice.cpp
