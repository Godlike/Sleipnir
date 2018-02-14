# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)

project(Sleipnir)

option(SLEIPNIR_BUILD_DOCUMENTATION "Build documentation" OFF)
option(SLEIPNIR_BUILD_DEMOS "Build demo projects" OFF)
option(SLEIPNIR_BUILD_TESTS "Build tests" OFF)
option(SLEIPNIR_SHLIBDEPS "Calculate shared library dependencies (for .deb only)" ON)

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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../MuleUtilities:$ORIGIN/../EponaMath:$ORIGIN/../ArionCollision")
set(CMAKE_DEBUG_POSTFIX "")

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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
if (UNIX)
    message(STATUS "SLEIPNIR_SHLIBDEPS: ${SLEIPNIR_SHLIBDEPS}")
endif()
message(STATUS "Version: ${SLEIPNIR_VERSION}")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.in
    ${CMAKE_CURRENT_BINARY_DIR}/version

    @ONLY
)

# Mule
message("======================================")
message("== Preparing Mule utilities library ==")
message("======================================")

set(MULE_BUILD_DOCUMENTATION ${SLEIPNIR_BUILD_DOCUMENTATION} CACHE BOOL "Build Mule documentation")
set(MULE_BUILD_TESTS OFF CACHE BOOL "Build Mule tests")
set(MULE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/godlike/Mule" CACHE STRING "Mule root directory.")

add_subdirectory(${MULE_ROOT})

find_package(Mule)
if (Mule_FOUND)
    include(${Mule_CONFIG_DIR}/MuleUtilitiesConfig.cmake)
endif()

# Unicorn Render
message("========================================")
message("== Preparing Unicorn rendering engine ==")
message("========================================")

set(UNICORN_BUILD_DOCUMENTATION ${SLEIPNIR_BUILD_DOCUMENTATION} CACHE BOOL "Build UnicornRender documentation")
set(UNICORN_BUILD_DEMOS ${SLEIPNIR_BUILD_DEMOS} CACHE BOOL "Build UnicornRender demo projects")
set(UNICORN_BUILD_TESTS ${SLEIPNIR_BUILD_TESTS} CACHE BOOL "Build UnicornRender tests")
set(UNICORN_CUSTOM_PACKAGING ON CACHE BOOL "Prepare custom CPack configuration file")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Unicorn)

# Epona
message("==================================")
message("== Preparing Epona math library ==")
message("==================================")

set(EPONA_BUILD_DEMOS OFF CACHE BOOL "Build Epona demo.")
set(EPONA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/godlike/Epona" CACHE STRING "Epona root directory.")

add_subdirectory(${EPONA_ROOT})

find_package(Epona)
if (Epona_FOUND)
    include(${Epona_CONFIG_DIR}/EponaConfig.cmake)
endif()

# Arion
message("=======================================")
message("== Preparing Arion collision library ==")
message("=======================================")

set(ARION_BUILD_DEMOS OFF CACHE BOOL "Build Arion demo.")
set(ARION_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/godlike/Arion" CACHE STRING "Arion root directory.")

add_subdirectory(${ARION_ROOT})

find_package(Arion)
if (Arion_FOUND)
    include(${Arion_CONFIG_DIR}/ArionConfig.cmake)
endif()

# Pegasus
message("======================================")
message("== Preparing Pegasus physics engine ==")
message("======================================")

set(PEGASUS_BUILD_DEMOS OFF CACHE BOOL "Build Pegasus demo.")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/godlike/Pegasus)

# Install information
message("=====================================")
message("== Preparing packaging information ==")
message("=====================================")

set(HAS_NSIS FALSE)

if (WIN32)
    find_package(NSIS)

    set(HAS_NSIS NSIS_FOUND)
endif()

set(SLEIPNIR_PACKAGE_NAME)

if (UNIX)
    set(SLEIPNIR_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${SLEIPNIR_MAINTAINER})
    set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)

    set(CPACK_INSTALL_CMAKE_PROJECTS
        "${CMAKE_CURRENT_BINARY_DIR};Mule Headers;mule_headers;/"
        "${CMAKE_CURRENT_BINARY_DIR};Mule Libraries;mule_libraries;/"
        "${CMAKE_CURRENT_BINARY_DIR};Unicorn Data;unicorn_data;/"
        "${CMAKE_CURRENT_BINARY_DIR};Unicorn Headers;unicorn_headers;/"
        "${CMAKE_CURRENT_BINARY_DIR};Unicorn Extra;unicorn_extra;/"
        "${CMAKE_CURRENT_BINARY_DIR};Unicorn Libraries;unicorn_libraries;/"
        "${CMAKE_CURRENT_BINARY_DIR};Pegasus Headers;pegasus_headers;/"
        "${CMAKE_CURRENT_BINARY_DIR};Pegasus Extra;pegasus_extra;/"
        "${CMAKE_CURRENT_BINARY_DIR};Pegasus Libraries;pegasus_libraries;/"
        "${CMAKE_CURRENT_BINARY_DIR};Arion Headers;arion_headers;/"
        "${CMAKE_CURRENT_BINARY_DIR};Arion Extra;arion_extra;/"
        "${CMAKE_CURRENT_BINARY_DIR};Arion Libraries;arion_libraries;/"
        "${CMAKE_CURRENT_BINARY_DIR};Epona Headers;epona_headers;/"
        "${CMAKE_CURRENT_BINARY_DIR};Epona Extra;epona_extra;/"
        "${CMAKE_CURRENT_BINARY_DIR};Epona Libraries;epona_libraries;/"
    )

    if (SLEIPNIR_SHLIBDEPS)
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    else()
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS
            "libc6 (>= 2.14), libgcc1 (>= 1:3.0), libstdc++6 (>= 5.2), libvulkan1, libx11-6 (>= 2:1.2.99.901), libxcursor1 (>> 1.1.2), libxinerama1, libxrandr2 (>= 2:1.2.99.3)"
        )
    endif()

    set(SLEIPNIR_PACKAGE_NAME "${SLEIPNIR_NAME}-${CMAKE_CXX_COMPILER_ID}")

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(SLEIPNIR_PACKAGE_NAME ${SLEIPNIR_PACKAGE_NAME}-dbg)
    endif()
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

if (NOT SLEIPNIR_PACKAGE_NAME)
    set(SLEIPNIR_PACKAGE_NAME ${SLEIPNIR_NAME}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE})
endif()

# Prepare install package
set(CPACK_PACKAGE_NAME ${SLEIPNIR_PACKAGE_NAME})
set(CPACK_PACKAGE_VENDOR ${SLEIPNIR_VENDOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${SLEIPNIR_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${SLEIPNIR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${SLEIPNIR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${SLEIPNIR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${SLEIPNIR_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${SLEIPNIR_INSTALL_DIRECTORY})

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

set(CPACK_GENERATOR ${SLEIPNIR_GENERATOR})

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

set(CPACK_COMPONENTS_ALL
    mule_headers
    mule_libraries

    unicorn_data
    unicorn_headers
    unicorn_extra
    unicorn_libraries

    pegasus_headers
    pegasus_extra
    pegasus_libraries

    arion_headers
    arion_extra
    arion_libraries

    epona_headers
    epona_extra
    epona_libraries
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

cpack_add_component(mule_libraries
    DISPLAY_NAME "Mule Library"
    DESCRIPTION "Mule Utilities library"
    GROUP "Runtime"
)

cpack_add_component(mule_headers
    DISPLAY_NAME "Mule Headers"
    DESCRIPTION "Headers for Mule Utilities library. Depends on Mule Library."
    GROUP "Development"
    DEPENDS mule_libraries
)

cpack_add_component(unicorn_data
    DISPLAY_NAME "Unicorn Data"
    DESCRIPTION "Basic shaders"
    GROUP "Runtime"
)

cpack_add_component(unicorn_libraries
    DISPLAY_NAME "Unicorn Library"
    DESCRIPTION "Unicorn Render library. Depends on Mule Library."
    GROUP "Runtime"
    DEPENDS mule_libraries
)

cpack_add_component(unicorn_headers
    DISPLAY_NAME "Unicorn Headers"
    DESCRIPTION "Headers for Unicorn Render library. Depends on Unicorn Library, Unicorn Extra and Mule Headers."
    GROUP "Development"
    DEPENDS unicorn_libraries unicorn_extra mule_headers
)

cpack_add_component(unicorn_extra
    DISPLAY_NAME "Unicorn Extra"
    DESCRIPTION "All needed third-party headers for Unicorn Headers"
    GROUP "Development"
)

cpack_add_component(pegasus_libraries
    DISPLAY_NAME "Pegasus Library"
    DESCRIPTION "Pegasus Physics library. Depends on Arion Library."
    GROUP "Runtime"
    DEPENDS arion_libraries
)

cpack_add_component(pegasus_headers
    DISPLAY_NAME "Pegasus Headers"
    DESCRIPTION "Headers for Pegasus Physics library. Depends on Pegasus Library, Pegasus Extra and Arion Headers."
    GROUP "Development"
    DEPENDS pegasus_libraries pegasus_extra arion_headers
)

cpack_add_component(pegasus_extra
    DISPLAY_NAME "Pegasus Extra"
    DESCRIPTION "All needed third-party headers for Pegasus Headers"
    GROUP "Development"
)

cpack_add_component(arion_libraries
    DISPLAY_NAME "Arion Library"
    DESCRIPTION "Arion Collision library. Depends on Epona Library."
    GROUP "Runtime"
    DEPENDS epona_libraries
)

cpack_add_component(arion_headers
    DISPLAY_NAME "Arion Headers"
    DESCRIPTION "Headers for Arion Collision library. Depends on Arion Library and Arion Extra."
    GROUP "Development"
    DEPENDS arion_libraries arion_extra
)

cpack_add_component(arion_extra
    DISPLAY_NAME "Arion Extra"
    DESCRIPTION "All needed third-party headers for Arion Headers"
    GROUP "Development"
)

cpack_add_component(epona_libraries
    DISPLAY_NAME "Epona Library"
    DESCRIPTION "Epona Math library"
    GROUP "Runtime"
)

cpack_add_component(epona_headers
    DISPLAY_NAME "Epona Headers"
    DESCRIPTION "Headers for Epona Math library. Depends on Epona Library and Epona Extra."
    GROUP "Development"
    DEPENDS epona_libraries epona_extra
)

cpack_add_component(epona_extra
    DISPLAY_NAME "Epona Extra"
    DESCRIPTION "All needed third-party headers for Epona Headers"
    GROUP "Development"
)

include(CPack)

# Perform install of needed components
add_custom_target( mock_install
    DEPENDS ${MULE_UTILITIES_LIB} ${UNICORN_RENDER_LIB} ${EPONA_MATH_LIB} ${ARION_COLLISION_LIB} ${PEGASUS_PHYSICS_LIB}

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=mule_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=mule_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_data -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=unicorn_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=epona_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=epona_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=epona_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=arion_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=arion_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=arion_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_headers -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_extra -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=${CMAKE_BUILD_TYPE} -DCOMPONENT=pegasus_libraries -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
)
