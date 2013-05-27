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
    OscInMessage.h \
    OscMessageFilter.h \
    Predicate.h \
    OscReceiver.h
	
SOURCES += \
    OscInMessage.cpp \
    OscMessageFilter.cpp \
    Predicate.cpp \
    OscReceiver.cpp

INCLUDEPATH += $$PWD/../../lib/oscpack/include
win32:LIBS += -L$$PWD/../../lib/oscpack/lib/win32/ -loscpack
else:macx:LIBS += -L$$PWD/../../lib/oscpack/lib/macx/ -loscpack
else:unix:LIBS += -L$$PWD/../../lib/oscpack/lib/linux/ -loscpack

INCLUDEPATH += $$PWD/../../lib/boost
