cmake_minimum_required(VERSION 3.16)
project(caspar VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(caspar
    STATIC
    AmcpDevice.cpp AmcpDevice.h
    CasparDevice.cpp CasparDevice.h
    Models/CasparData.cpp Models/CasparData.h
    Models/CasparMedia.cpp Models/CasparMedia.h
    Models/CasparTemplate.cpp Models/CasparTemplate.h
    Models/CasparThumbnail.cpp Models/CasparThumbnail.h
    Shared.h
)
add_external_dependencies(caspar)
target_include_directories(caspar PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../Common
    ../Common
)

target_compile_definitions(caspar PUBLIC
    CASPAR_LIBRARY
)

target_link_libraries(caspar PUBLIC
    common

    Qt::Core
    Qt::Network
)
