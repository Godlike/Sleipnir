# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)

project(sleipnir)

option(SLEIPNIR_BUILD_DOCUMENTATION "Build documentation" OFF)
option(SLEIPNIR_BUILD_DEMOS "Build demo projects" ON)
option(SLEIPNIR_BUILD_TESTS "Build tests" ON)

# Setup output folders if none were specified
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
endif()

set(BUILD_SHARED_LIBS ON CACHE BOOL "Flag indicating if we want to build shared libraries")

# Unicorn Render
message("========================================")
message("== Preparing Unicorn rendering engine ==")
message("========================================")

set(UNICORN_BUILD_DOCUMENTATION ${SLEIPNIR_BUILD_DOCUMENTATION})
set(UNICORN_BUILD_DEMOS ${SLEIPNIR_BUILD_DEMOS})
set(UNICORN_BUILD_TESTS ${SLEIPNIR_BUILD_TESTS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Unicorn)

# Pegasus
message("======================================")
message("== Preparing Pegasus physics engine ==")
message("======================================")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Pegasus)

# Prepare install package
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Godlike")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Yet another game engine")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Godlike")

if (UNIX)
    set(CPACK_GENERATOR "DEB;ZIP")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Godlike")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
# set(CPACK_COMPONENTS_ALL godlike_data godlike_headers godlike_libraries)

set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_CURRENT_BINARY_DIR};data;godlike_data;."
    "${CMAKE_CURRENT_BINARY_DIR};headers;godlike_headers;."
    "${CMAKE_CURRENT_BINARY_DIR};libraries;godlike_libraries;."
)

include(CPack)

add_custom_target(
    mock_install
    DEPENDS ${UNICORN_RENDER_LIB} ${PEGASUS_PHYSICS_LIB}
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=godlike_data -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=godlike_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=godlike_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
)
