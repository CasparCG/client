QT -= gui
QT += network

CONFIG += c++11

TARGET = sony
TEMPLATE = lib

DEFINES += SONY_LIBRARY _GLIBCXX_USE_CXX11_ABI=0

HEADERS += \
    Shared.h \
    ViscaDevice.h
	
SOURCES += \
    ViscaDevice.cpp
