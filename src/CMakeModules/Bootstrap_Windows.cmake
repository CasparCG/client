function (add_external_dependencies PROJECT)
    add_dependencies(${PROJECT} boost)
endfunction()

set(BOOST_USE_PRECOMPILED ON CACHE BOOL "Use precompiled boost")

# BOOST
if (BOOST_USE_PRECOMPILED)
	ExternalProject_Add(boost
		URL ${CASPARCG_DOWNLOAD_MIRROR}/boost/boost_1_83_0-win32-x64-debug-release.zip
		URL_HASH MD5=0b9990a24259867c8c04ae30c423f86b
		DOWNLOAD_DIR ${CASPARCG_DOWNLOAD_CACHE}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)
	ExternalProject_Get_Property(boost SOURCE_DIR)
	include_directories("${SOURCE_DIR}/include/boost-1_83")
	link_directories("${SOURCE_DIR}/lib")
else ()
	set(BOOST_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost-install)
	ExternalProject_Add(boost
		URL ${CASPARCG_DOWNLOAD_MIRROR}/boost/boost_1_83_0.zip
		URL_HASH MD5=03d5aea72401ffed848cb5daf8cd2b9b
		DOWNLOAD_DIR ${CASPARCG_DOWNLOAD_CACHE}
		BUILD_IN_SOURCE 1
		CONFIGURE_COMMAND ./bootstrap.bat
			--with-libraries=filesystem
			--with-libraries=locale
			--with-libraries=log
			--with-libraries=log_setup
			--with-libraries=regex
			--with-libraries=system
			--with-libraries=thread
		BUILD_COMMAND ./b2 install debug release --prefix=${BOOST_INSTALL_DIR} link=static threading=multi runtime-link=shared -j ${CONFIG_CPU_COUNT} 
		INSTALL_COMMAND ""
	)
    include_directories("${BOOST_INSTALL_DIR}/include/boost-1_83")
	link_directories("${BOOST_INSTALL_DIR}/lib")
endif ()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT shell)

set (CMAKE_PREFIX_PATH "${BUILD_QT_PATH}")
