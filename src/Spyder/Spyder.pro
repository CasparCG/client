#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T13:07:49
#
#-------------------------------------------------
QT -= gui
QT += network

TARGET = spyder
TEMPLATE = lib

DEFINES += SPYDER_LIBRARY

HEADERS += \
    Shared.h \
    SpyderDevice.h
	
SOURCES += \
    SpyderDevice.cpp
