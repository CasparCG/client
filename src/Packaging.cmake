set(CPACK_PACKAGE_VENDOR "CasparCG")
set(CPACK_PACKAGE_NAME "CasparCG-Client")
set(CPACK_PACKAGE_CONTACT "Julian Waller <julian@superfly.tv>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/../README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}")

# set human names to exetuables
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "Example Apps")
set(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME}")
set(CPACK_STRIP_FILES TRUE)

#------------------------------------------------------------------------------
# include CPack, so we get target for packages
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/BundleConfig.cmake")


add_custom_target(bundle
                  COMMAND ${CMAKE_CPACK_COMMAND} "--config" "${CMAKE_BINARY_DIR}/BundleConfig.cmake"
                  COMMENT "Running CPACK. Please wait..."
                  DEPENDS ${PROJECT_NAME})
set(CPACK_GENERATOR "")

if(WIN32 AND NOT UNIX)
    #--------------------------------------------------------------------------
    # Windows specific

    # copy dependencies next to executable after build
    add_custom_command(TARGET shell
        POST_BUILD
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}/bin
        COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}" "$<TARGET_FILE_DIR:shell>/$<TARGET_FILE_NAME:shell>"
        # COMMAND Qt6::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}" "$<TARGET_FILE_DIR:shell>/$<TARGET_FILE_NAME:shell>"
    )

    # list(APPEND CPACK_GENERATOR ZIP)
    # message(STATUS "Package generation - Windows")
    # message(STATUS "   + ZIP                                  YES ")
    
    # set(PACKAGE_ICON "${CMAKE_SOURCE_DIR}/resources/icon.ico")

    # # NSIS windows installer
    # find_program(NSIS_PATH nsis PATH_SUFFIXES nsis)
    # if(NSIS_PATH)
    #     list(APPEND CPACK_GENERATOR NSIS)
    #     message(STATUS "   + NSIS                                 YES ")

    #     set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
    #     # Icon of the installer
    #     file(TO_NATIVE_PATH "${PACKAGE_ICON}" CPACK_NSIS_MUI_ICON)
    #     file(TO_NATIVE_PATH "${PACKAGE_ICON}" CPACK_NSIS_MUI_HEADERIMAGE_BITMAP)
    #     set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
    #     set(CPACK_NSIS_MODIFY_PATH ON)
    # else()
    #     message(STATUS "   + NSIS                                 NO ")
    # endif()

    # # NuGet package
    # # find_program(NUGET_EXECUTABLE nuget)
    # set(NUGET_EXECUTABLE OFF)
    # if(NUGET_EXECUTABLE)
    #     list(APPEND CPACK_GENERATOR NuGET)
    #     message(STATUS "   + NuGET                               YES ")
    #     set(CPACK_NUGET_PACKAGE_NAME "${PROJECT_NAME}")
	# set(CPACK_NUGET_PACKAGE_VERSION "1.0.0")
	# set(CPACK_NUGET_PACKAGE_DESCRIPTION "Example")
	# set(CPACK_NUGET_PACKAGE_AUTHORS "Example")
    # else()
    #     message(STATUS "   + NuGET                                NO ")
    # endif()

    # windeployqt(${PROJECT_NAME})

elseif(APPLE)
    #--------------------------------------------------------------------------
    # Apple specific
    message(STATUS "Package generation - Mac OS X")

    qt_generate_deploy_app_script(
        TARGET shell
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})

else()

    #-----------------------------------------------------------------------------
    # Linux specific
    message(STATUS "Package generation - UNIX")
    # list(APPEND CPACK_GENERATOR TGZ)
    # message(STATUS "   + TGZ                                  YES ")
    # message(STATUS "   + TXZ                                  YES ")

    list(APPEND CPACK_GENERATOR DEB)
    message(STATUS "   + DEB                                  YES ")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_URL}")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)  # ON When build with libraries only from debian packages

    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/Shell/Images/CasparCG.png")

    INSTALL(FILES "${CMAKE_SOURCE_DIR}/../resources/linux/casparcg-client.desktop" DESTINATION share/applications)
    INSTALL(FILES "${CMAKE_SOURCE_DIR}/Shell/Images/CasparCG.png" DESTINATION share/icons/casparcg-client/)

endif()

include(CPack)