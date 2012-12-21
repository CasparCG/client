#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:26:10
#
#-------------------------------------------------

QT += core

TARGET = gpi
TEMPLATE = lib

DEFINES += GPI_LIBRARY

HEADERS += \
    GpiDevice.h \
    Shared.h

SOURCES += \
    GpiDevice.cpp

INCLUDEPATH += $$PWD/../../dependencies/boost
win32: LIBS += -L$$PWD/../../dependencies/boost/stage/lib/win32/ -lboost_date_time-mgw44-mt-1_47 -lboost_system-mgw44-mt-1_47 -lboost_thread-mgw44-mt-1_47 -lboost_filesystem-mgw44-mt-1_47 -lboost_chrono-mgw44-mt-1_47 -lws2_32
else:macx: LIBS += -L$$PWD/../../dependencies/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono

DEPENDPATH += $$PWD/../../dependencies/gpio-client/include
INCLUDEPATH += $$PWD/../../dependencies/gpio-client/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/gpio-client/lib/win32/release/ -lgpio-client
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/gpio-client/lib/win32/debug/ -lgpio-client
else:macx: LIBS += -L$$PWD/../../dependencies/gpio-client/lib/macx/ -lgpio-client
