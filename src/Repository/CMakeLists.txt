cmake_minimum_required(VERSION 3.16)
project(repository VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

qt_add_library(repository
    STATIC
    Models/RepositoryChangeModel.cpp Models/RepositoryChangeModel.h
    RepositoryDevice.cpp RepositoryDevice.h
    RrupDevice.cpp RrupDevice.h
    Shared.h
)
add_external_dependencies(repository)
target_compile_definitions(repository PUBLIC
    REPOSITORY_LIBRARY
)

target_link_libraries(repository PUBLIC
    Qt::Core
    Qt::Network
    Qt::Core5Compat
)
