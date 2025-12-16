cmake_host_system_information(RESULT CONFIG_CPU_COUNT QUERY NUMBER_OF_PHYSICAL_CORES)

function (add_external_dependencies PROJECT)
    add_dependencies(${PROJECT} boost)
endfunction()

if (NOT Qt6_ROOT)
    message(FATAL_ERROR "You must specify a root value to the qt6 installation -DQt6_ROOT=$HOME/Qt/6.2.0/macos")
endif()

set(BOOST_USE_PRECOMPILED ON CACHE BOOL "Use precompiled boost")

# BOOST
# casparcg_add_external_project(boost)
if (BOOST_USE_PRECOMPILED)
	ExternalProject_Add(boost
		URL ${CASPARCG_DOWNLOAD_MIRROR}/boost/boost_1_83_0-macos-multiarch-release.zip
		URL_HASH MD5=029541b7a7d6827fc0d79b1822b3b3be
		DOWNLOAD_DIR ${CASPARCG_DOWNLOAD_CACHE}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
	)
	ExternalProject_Get_Property(boost SOURCE_DIR)
	set(BOOST_INCLUDE_PATH "${SOURCE_DIR}/include")
    include_directories(${BOOST_INCLUDE_PATH}) # TODO this is not pretty
	link_directories("${SOURCE_DIR}/lib")
else ()
	set(BOOST_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost-install)
	ExternalProject_Add(boost
		URL ${CASPARCG_DOWNLOAD_MIRROR}/boost/boost_1_83_0.zip
		URL_HASH MD5=03d5aea72401ffed848cb5daf8cd2b9b
		DOWNLOAD_DIR ${CASPARCG_DOWNLOAD_CACHE}
		BUILD_IN_SOURCE 1
		CONFIGURE_COMMAND ./bootstrap.sh
			--with-libraries=thread
			--with-libraries=system
		BUILD_COMMAND ./b2 install release architecture=arm+x86 --prefix=${BOOST_INSTALL_DIR} link=static -j ${CONFIG_CPU_COUNT} 
		INSTALL_COMMAND ""
	)
	set(BOOST_INCLUDE_PATH "${BOOST_INSTALL_DIR}/include")
	link_directories("${BOOST_INSTALL_DIR}/lib")
    include_directories(${BOOST_INCLUDE_PATH}) # TODO this is not pretty
endif ()
set(Boost_LIBRARIES "boost_thread")
add_definitions( -D_HAS_AUTO_PTR_ETC=0 ) # fix incompatibility with c++17
add_definitions( -DBOOST_ERROR_CODE_HEADER_ONLY ) # fix a link issue inside of 

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-deprecated-builtins -Wno-deprecated-declarations") # fix some noisey deprecation warnings in boost
