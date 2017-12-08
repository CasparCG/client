QT += core network websockets

CONFIG += c++11

TARGET = osc
TEMPLATE = lib

DEFINES += OSC_LIBRARY _GLIBCXX_USE_CXX11_ABI=0 QT_COMPILING_QSTRING_COMPAT_CPP

HEADERS += \
    Shared.h \
    OscThread.h \
    OscListener.h \
    OscSender.h \
    OscWebSocketListener.h

SOURCES += \
    OscThread.cpp \
    OscListener.cpp \
    OscSender.cpp \
    OscWebSocketListener.cpp

CONFIG(system-boost) {
    LIBS += -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono
} else {
    DEPENDPATH += $$PWD/../../lib/boost
    INCLUDEPATH += $$PWD/../../lib/boost
    win32:LIBS += -L$$PWD/../../lib/boost/stage/lib/win32/ -lboost_date_time-mgw49-mt-1_57 -lboost_system-mgw49-mt-1_57 -lboost_thread-mgw49-mt-1_57 -lboost_filesystem-mgw49-mt-1_57 -lboost_chrono-mgw49-mt-1_57 -lws2_32
    else:macx:LIBS += -L$$PWD/../../lib/boost/stage/lib/macx/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono
    else:unix:LIBS += -L$$PWD/../../lib/boost/stage/lib/linux/ -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono
}

CONFIG(system-oscpack) {
    LIBS += -loscpack
} else {
    DEPENDPATH += $$PWD/../../lib/oscpack/include
    INCLUDEPATH += $$PWD/../../lib/oscpack/include
    win32:LIBS += -L$$PWD/../../lib/oscpack/lib/win32/ -loscpack
    else:macx:LIBS += -L$$PWD/../../lib/oscpack/lib/macx/ -loscpack
    else:unix:LIBS += -L$$PWD/../../lib/oscpack/lib/linux/ -loscpack
}
