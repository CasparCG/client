QT -= gui
QT += network

CONFIG += c++11

TARGET = tricaster
TEMPLATE = lib

DEFINES += TRICASTER_LIBRARY _GLIBCXX_USE_CXX11_ABI=0

HEADERS += \
    TriCasterDevice.h \
    Shared.h \
    NtfcDevice.h
	
SOURCES += \
    TriCasterDevice.cpp \
    NtfcDevice.cpp
