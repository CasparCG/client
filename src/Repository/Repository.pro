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
    Models/RepositoryRemove.h \
    Models/RepositoryAdd.h \
    RrupDevice.h
	
SOURCES += \
    RepositoryDevice.cpp \
    Models/RepositoryRemove.cpp \
    Models/RepositoryAdd.cpp \
    RrupDevice.cpp
