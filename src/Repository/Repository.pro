#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T13:07:49
#
#-------------------------------------------------
QT -= gui
QT += network

TARGET = repository
TEMPLATE = lib

DEFINES += REPOSITORY_LIBRARY

HEADERS += \
    Shared.h \
    RepositoryDevice.h \
    RrupDevice.h \
    Models/RepositoryChangeModel.h
	
SOURCES += \
    RepositoryDevice.cpp \
    RrupDevice.cpp \
    Models/RepositoryChangeModel.cpp
