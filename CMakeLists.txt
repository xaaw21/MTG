cmake_minimum_required(VERSION 3.6)

cmake_policy(SET CMP0048 NEW) # enable PROJECT_VERSION

include(CMakePackageConfigHelpers)

#options
option(BUILD_STATIC_ENGINE "Set to OFF|ON (default is ON) to control build Engine as STATIC library" ON)
option(BUILD_TESTS "Set to OFF|ON (default is OFF) to build tests" OFF)

PROJECT(MTG VERSION 1.0.0)

#conan
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

if("${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" STREQUAL "")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

if("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

if("${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" STREQUAL "")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

if(BUILD_STATIC)
    set(LIB_MODE STATIC)
    add_definitions(-DMTG_STATIC)
    message(STATUS "Building static libraries")
else(BUILD_STATIC)
    set(LIB_MODE SHARED)
    add_definitions(-DMTG_SHARED)
    message(STATUS "Building dynamic libraries")
endif(BUILD_STATIC)

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
set(TYPE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
set(TYPE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
else()
set(TYPE_POSTFIX "")
endif()

if(DEFINED MSVC) 
set(CMAKE_CXX_FLAGS "/EHsc")
else()
set(CMAKE_CXX_FLAGS "-std=c++11 -U__STRICT_ANSI__")
endif(DEFINED MSVC)

set(PROJECT_TARGETS "${PROJECT_NAME}Targets")
set(PROJECT_CONFIG "${PROJECT_NAME}Config.cmake")
set(PROJECT_CONFIG_VERSION "${PROJECT_NAME}ConfigVersion.cmake")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR "" CACHE PATH "Installation directory for header files")
set(INSTALL_SRC_DIR "" CACHE PATH "Installation directory for source files")
set(INSTALL_CMAKE_DIR cmake CACHE PATH "Installation directory for CMake files")
set(EXPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_TARGETS}.cmake)

# install
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_CONFIG}.in "${PROJECT_CONFIG}" INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION}" VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG}" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION}" DESTINATION "${INSTALL_CMAKE_DIR}")
install(EXPORT ${PROJECT_TARGETS} FILE ${PROJECT_TARGETS}.cmake DESTINATION "${INSTALL_CMAKE_DIR}")


LINK_DIRECTORIES(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_subdirectory(src)

if(BUILD_TESTS)
	add_subdirectory(tests)
endif()