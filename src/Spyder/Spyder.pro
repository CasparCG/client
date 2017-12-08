QT -= gui
QT += network

CONFIG += c++11

TARGET = spyder
TEMPLATE = lib

DEFINES += SPYDER_LIBRARY _GLIBCXX_USE_CXX11_ABI=0

HEADERS += \
    Shared.h \
    SpyderDevice.h
	
SOURCES += \
    SpyderDevice.cpp
