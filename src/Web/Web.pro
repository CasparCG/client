QT -= gui
QT += network

CONFIG += c++11

TARGET = web
TEMPLATE = lib

DEFINES += WEB_LIBRARY

HEADERS += \
    Shared.h \
    HttpRequest.h
	
SOURCES += \
    HttpRequest.cpp
