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
    OscThread.h \
    OscListener.h \
    OscSender.h
	
SOURCES += \
    OscThread.cpp \
    OscListener.cpp \
    OscSender.cpp

DEPENDPATH += $$PWD/../../lib/boost
INCLUDEPATH += $$PWD/../../lib/boost
win32:LIBS += -L$$PWD/../../lib/boost/stage/lib/win32/ -lboost_date_time-mgw44-mt-1_47 -lboost_system-mgw44-mt-1_47 -lboost_thread-mgw44-mt-1_47 -lboost_filesystem-mgw44-mt-1_47 -lboost_chrono-mgw44-mt-1_47 -lws2_32
else:macx:LIBS += -L$$PWD/../../lib/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono
else:unix:LIBS += -L$$PWD/../../lib/boost/stage/lib/linux/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono

DEPENDPATH += $$PWD/../../lib/oscpack/include
INCLUDEPATH += $$PWD/../../lib/oscpack/include
win32:LIBS += -L$$PWD/../../lib/oscpack/lib/win32/ -loscpack
else:macx:LIBS += -L$$PWD/../../lib/oscpack/lib/macx/ -loscpack
else:unix:LIBS += -L$$PWD/../../lib/oscpack/lib/linux/ -loscpack
