#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:07:49
#
#-------------------------------------------------
QT -= gui
QT += network

TARGET = caspar
TEMPLATE = lib

DEFINES += CASPAR_LIBRARY

HEADERS += \
    CasparDevice.h \
    Shared.h \
    Models/CasparThumbnail.h \
    Models/CasparTemplate.h \
    Models/CasparMedia.h \
    Models/CasparData.h \
    AmcpDevice.h
	
SOURCES += \
    CasparDevice.cpp \
    Models/CasparThumbnail.cpp \
    Models/CasparTemplate.cpp \
    Models/CasparMedia.cpp \
    Models/CasparData.cpp \
    AmcpDevice.cpp
