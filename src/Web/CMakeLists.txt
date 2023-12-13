cmake_minimum_required(VERSION 3.16)
project(web VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(web
    STATIC
    HttpRequest.cpp HttpRequest.h
    Shared.h
)
add_external_dependencies(web)
target_compile_definitions(web PUBLIC
    WEB_LIBRARY
)

target_link_libraries(web PUBLIC
    Qt::Core
    Qt::Network
)
