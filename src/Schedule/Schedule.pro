QT += core

CONFIG += c++11

TARGET = schedule
TEMPLATE = lib

DEFINES += SCHEDULE_LIBRARY

SOURCES += \
    ScheduleThread.cpp \
    ScheduleWorker.cpp \
    ScheduleWorkerBase.cpp

HEADERS +=\
    Shared.h \
    ScheduleThread.h \
    ScheduleWorker.h \
    ScheduleWorkerBase.h

