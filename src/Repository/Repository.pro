QT -= gui
QT += network

CONFIG += c++11

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
