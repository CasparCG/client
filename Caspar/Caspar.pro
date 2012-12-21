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
    CasparVersion.h \
    CasparTemplate.h \
    CasparMedia.h \
    CasparData.h \
    Shared.h
	
SOURCES += \
    CasparDevice.cpp \
    AMCPDevice.cpp \
    CasparVersion.cpp \
    CasparTemplate.cpp \
    CasparMedia.cpp \
    CasparData.cpp
