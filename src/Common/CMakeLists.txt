cmake_minimum_required(VERSION 3.16)
project(common VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(common
    STATIC
    Global.h
    Playout.cpp Playout.h
    Shared.h
    Timecode.cpp Timecode.h
    Xml.cpp Xml.h
)
add_external_dependencies(common)
target_compile_definitions(common PUBLIC
    COMMON_LIBRARY
)

target_link_libraries(common PUBLIC
    Qt::Core
    Qt::Gui
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
