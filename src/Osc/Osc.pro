#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:26:10
#
#-------------------------------------------------

QT += core

TARGET = osc
TEMPLATE = lib

DEFINES += OSC_LIBRARY

HEADERS += \
    Shared.h \
    OscDevice.h
	
SOURCES += \
    OscDevice.cpp

DEPENDPATH += $$PWD/../../lib/oscpack/include
INCLUDEPATH += $$PWD/../../lib/oscpack/include
win32:LIBS += -L$$PWD/../../lib/oscpack/lib/win32/ -loscpack
else:macx:LIBS += -L$$PWD/../../lib/oscpack/lib/macx/ -loscpack
else:unix:LIBS += -L$$PWD/../../lib/oscpack/lib/linux/ -loscpack
