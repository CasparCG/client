cmake_minimum_required(VERSION 3.16)
project(shell VERSION 1.0 LANGUAGES C CXX)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/../staging" CACHE PATH "Install path" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Sql Widgets Core5Compat Multimedia MultimediaWidgets)

if (UNIX AND APPLE)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Images/CasparCG.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

    set(OS_SPECIFIC_SOURCES
        Mac/AppNap.h
        Mac/AppNap.mm
        Images/CasparCG.icns
    )
elseif(WIN32)
    set(OS_SPECIFIC_SOURCES
        Windows.rc
    )
endif ()

qt_add_executable(shell WIN32 MACOSX_BUNDLE
    Application.cpp Application.h
    ${OS_SPECIFIC_SOURCES}
    Main.cpp
)
add_external_dependencies(shell)
set_target_properties(shell PROPERTIES 
    OUTPUT_NAME "casparcg-client"
    INSTALL_RPATH "$ORIGIN"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/../resources/macos/Info.plist"
)

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_isMultiConfig)
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../Widgets/widgets_autogen/include_$<CONFIG>)
else()
  set(AUTOGEN_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../Widgets/widgets_autogen/include)
endif()

target_include_directories(shell PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/../Common
    ${CMAKE_CURRENT_BINARY_DIR}/../Widgets
    ${AUTOGEN_INCLUDE_DIR}
    ../Caspar
    ../Common
    ../Core
    ../Gpi
    ../Osc
    ../Repository
    ../Web
    ../Widgets
)

target_link_libraries(shell PRIVATE
    caspar
    common
    core
    gpi
    osc
    repository
    web
    widgets

    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Sql
    Qt::Widgets
    Qt::Core5Compat
    Qt::Multimedia

    ${Boost_LIBRARIES}
)

# Resources:
set(Shell_resource_files
    "Images/CasparCG.icns"
    "Images/CasparCG.ico"
)

qt_add_resources(shell "Shell"
    PREFIX
        "/Graphics"
    FILES
        ${Shell_resource_files}
)
set(Shell1_resource_files
    "Fonts/OpenSans-Bold.ttf"
    "Fonts/OpenSans-BoldItalic.ttf"
    "Fonts/OpenSans-ExtraBold.ttf"
    "Fonts/OpenSans-ExtraBoldItalic.ttf"
    "Fonts/OpenSans-Italic.ttf"
    "Fonts/OpenSans-Light.ttf"
    "Fonts/OpenSans-LightItalic.ttf"
    "Fonts/OpenSans-Regular.ttf"
    "Fonts/OpenSans-Semibold.ttf"
    "Fonts/OpenSans-SemiboldItalic.ttf"
)

qt_add_resources(shell "Shell1"
    PREFIX
        "/Appearances"
    FILES
        ${Shell1_resource_files}
)

if (UNIX AND APPLE)
    target_link_libraries(shell PRIVATE
        "-framework Foundation"
    )
endif()

install(TARGETS shell
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_target(casparcg_copy_dependencies ALL)
add_external_dependencies(casparcg_copy_dependencies)

set(OUTPUT_FOLDER "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
add_custom_command(TARGET casparcg_copy_dependencies POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory \"${OUTPUT_FOLDER}\")

include(${CMAKE_CURRENT_LIST_DIR}/../Packaging.cmake)
