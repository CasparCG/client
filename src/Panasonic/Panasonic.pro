QT -= gui
QT += network

CONFIG += c++11

TARGET = panasonic
TEMPLATE = lib

DEFINES += PANASONIC_LIBRARY _GLIBCXX_USE_CXX11_ABI=0

HEADERS += \
    Shared.h \
    PtzDevice.h
	
SOURCES += \
    PtzDevice.cpp
