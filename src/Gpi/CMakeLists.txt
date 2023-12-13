cmake_minimum_required(VERSION 3.16)
project(gpi VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(gpi
    STATIC
    GpiDevice.cpp GpiDevice.h
    gpio-client.cpp gpio-client.h gpio-device.h
    Shared.h
)
add_external_dependencies(gpi)
target_compile_definitions(gpi PUBLIC
    GPI_LIBRARY
)

target_link_libraries(gpi PUBLIC
    Qt::Core
    Qt::Gui

    ${Boost_LIBRARIES}
)
