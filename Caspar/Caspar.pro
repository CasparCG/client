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
    AMCPDevice.h \
    CasparTemplate.h \
    CasparMedia.h \
    Shared.h \
    CasparVersion.h \
    CasparThumbnail.h \
    CasparData.h
	
SOURCES += \
    CasparDevice.cpp \
    AMCPDevice.cpp \
    CasparTemplate.cpp \
    CasparMedia.cpp \
    CasparVersion.cpp \
    CasparThumbnail.cpp \
    CasparData.cpp
