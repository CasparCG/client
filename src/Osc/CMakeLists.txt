cmake_minimum_required(VERSION 3.16)
project(osc VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

set(OscPackRoot ../../lib/oscpack)
IF(WIN32)
 set(OscPackIpSystemTypePath ip/win32)
 set(OscPackLibs Ws2_32 winmm)
ELSE(WIN32)
 set(OscPackIpSystemTypePath ip/posix)
ENDIF(WIN32)

set(OscPackSources 
    ${OscPackRoot}/ip/IpEndpointName.h
    ${OscPackRoot}/ip/IpEndpointName.cpp

    ${OscPackRoot}/ip/NetworkingUtils.h
    ${OscPackRoot}/${OscPackIpSystemTypePath}/NetworkingUtils.cpp

    ${OscPackRoot}/ip/UdpSocket.h
    ${OscPackRoot}/${OscPackIpSystemTypePath}/UdpSocket.cpp

    ${OscPackRoot}/ip/PacketListener.h
    ${OscPackRoot}/ip/TimerListener.h

    ${OscPackRoot}/osc/OscTypes.h
    ${OscPackRoot}/osc/OscTypes.cpp 
    ${OscPackRoot}/osc/OscHostEndianness.h
    ${OscPackRoot}/osc/OscException.h
    ${OscPackRoot}/osc/OscPacketListener.h
    ${OscPackRoot}/osc/MessageMappingOscPacketListener.h
    ${OscPackRoot}/osc/OscReceivedElements.h
    ${OscPackRoot}/osc/OscReceivedElements.cpp
    ${OscPackRoot}/osc/OscPrintReceivedElements.h
    ${OscPackRoot}/osc/OscPrintReceivedElements.cpp
    ${OscPackRoot}/osc/OscOutboundPacketStream.h
    ${OscPackRoot}/osc/OscOutboundPacketStream.cpp
)


qt_add_library(osc
    STATIC
    OscControlListener.cpp OscControlListener.h
    OscMonitorListener.cpp OscMonitorListener.h
    OscSender.cpp OscSender.h
    OscThread.cpp OscThread.h
    OscWebSocketListener.cpp OscWebSocketListener.h
    Shared.h
    ${OscPackSources}
)
add_external_dependencies(osc)
target_compile_definitions(osc PUBLIC
    OSC_LIBRARY
)

target_link_libraries(osc PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::WebSockets

    ${OscPackLibs}
)

target_include_directories(osc PUBLIC
    ${OscPackRoot}
)
