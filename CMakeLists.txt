# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)

project(Sleipnir)

option(SLEIPNIR_BUILD_DOCUMENTATION "Build documentation" OFF)
option(SLEIPNIR_BUILD_DEMOS "Build demo projects" OFF)
option(SLEIPNIR_BUILD_TESTS "Build tests" OFF)

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

# Prepare install information
message("===============================")
message("== Preparing Sleipnir Engine ==")
message("===============================")

set(SLEIPNIR_NAME ${PROJECT_NAME})
set(SLEIPNIR_VENDOR "Godlike")
set(SLEIPNIR_MAINTAINER ${SLEIPNIR_VENDOR})
set(SLEIPNIR_DESCRIPTION "Yet another game engine")
set(SLEIPNIR_INSTALL_DIRECTORY "Godlike")

set(SLEIPNIR_VERSION_MAJOR 0)
set(SLEIPNIR_VERSION_MINOR 1)
set(SLEIPNIR_VERSION_PATCH 0)
set(SLEIPNIR_VERSION_TWEAK 0)

set(SLEIPNIR_VERSION "${SLEIPNIR_VERSION_MAJOR}.${SLEIPNIR_VERSION_MINOR}.${SLEIPNIR_VERSION_PATCH}")

message(STATUS "Version: ${SLEIPNIR_VERSION}")

# Unicorn Render
message("========================================")
message("== Preparing Unicorn rendering engine ==")
message("========================================")

set(UNICORN_BUILD_DOCUMENTATION ${SLEIPNIR_BUILD_DOCUMENTATION} CACHE BOOL "Build UnicornRender documentation")
set(UNICORN_BUILD_DEMOS ${SLEIPNIR_BUILD_DEMOS} CACHE BOOL "Build UnicornRender demo projects")
set(UNICORN_BUILD_TESTS ${SLEIPNIR_BUILD_TESTS} CACHE BOOL "Build UnicornRender tests")
set(UNICORN_CUSTOM_PACKAGING ON CACHE BOOL "Prepare custom CPack configuration file")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Unicorn)

# Pegasus
message("======================================")
message("== Preparing Pegasus physics engine ==")
message("======================================")

set(PEGASUS_BUILD_DEMOS OFF CACHE BOOL "Build Pegasus demo.")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Pegasus)

# Install information
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(HAS_NSIS FALSE)

if (WIN32)
    find_package(NSIS)

    set(HAS_NSIS NSIS_FOUND)
endif()

if (UNIX)
    set(SLEIPNIR_GENERATOR "DEB")
elseif (WIN32 AND HAS_NSIS)
    set(SLEIPNIR_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME ${SLEIPNIR_NAME})
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/NSIS.definitions.nsh.in
        ${CMAKE_CURRENT_BINARY_DIR}/resources/NSIS/NSIS.definitions.nsh
    )
else()
    set(SLEIPNIR_GENERATOR "ZIP")
endif()

# Prepare install package
set(CPACK_PACKAGE_NAME ${SLEIPNIR_NAME})
set(CPACK_PACKAGE_VENDOR ${SLEIPNIR_VENDOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${SLEIPNIR_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${SLEIPNIR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${SLEIPNIR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${SLEIPNIR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${SLEIPNIR_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${SLEIPNIR_INSTALL_DIRECTORY})

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

set(CPACK_GENERATOR ${SLEIPNIR_GENERATOR})

set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${SLEIPNIR_MAINTAINER})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

set(CPACK_COMPONENTS_ALL
    unicorn_data
    unicorn_headers
    unicorn_extra
    unicorn_libraries
    pegasus_headers
    pegasus_extra
    pegasus_libraries
)

include(CPackComponent)

# CPack component groups
cpack_add_component_group(runtime
    DESCRIPTION "Libraries and shaders"
)

cpack_add_component_group(development
    DESCRIPTION "Headers and third-party dependencies"
    EXPANDED
)

# CPack components
cpack_add_component(unicorn_data
    DISPLAY_NAME "Unicorn Data"
    DESCRIPTION "Basic shaders"
    GROUP "Runtime"
)

cpack_add_component(unicorn_libraries
    DISPLAY_NAME "Unicorn Library"
    DESCRIPTION "Unicorn Render library"
    GROUP "Runtime"
)

cpack_add_component(unicorn_headers
    DISPLAY_NAME "Unicorn Headers"
    DESCRIPTION "Headers for Unicorn Render library. Depends on Unicorn Library and Unicorn Extra."
    GROUP "Development"
    DEPENDS unicorn_libraries unicorn_extra
)

cpack_add_component(unicorn_extra
    DISPLAY_NAME "Unicorn Extra"
    DESCRIPTION "All needed third-party headers for Unicorn Headers"
    GROUP "Development"
)

cpack_add_component(pegasus_libraries
    DISPLAY_NAME "Pegasus Library"
    DESCRIPTION "Pegasus Physics library"
    GROUP "Runtime"
)

cpack_add_component(pegasus_headers
    DISPLAY_NAME "Pegasus Headers"
    DESCRIPTION "Headers for Pegasus Physics library. Depends on Pegasus Library and Pegasus Extra."
    GROUP "Development"
    DEPENDS pegasus_libraries pegasus_extra
)

cpack_add_component(pegasus_extra
    DISPLAY_NAME "Pegasus Extra"
    DESCRIPTION "All needed third-party headers for Pegasus Headers"
    GROUP "Development"
)

include(CPack)

# Perform install of needed components
add_custom_target( mock_install
    DEPENDS ${UNICORN_RENDER_LIB} ${PEGASUS_PHYSICS_LIB}

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_data -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
)
