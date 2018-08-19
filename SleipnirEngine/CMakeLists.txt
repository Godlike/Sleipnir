# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(${SLEIPNIR_ENGINE_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## Build flags
include(SleipnirBuildFlags)

## Installation info
include(SleipnirInstallInfo)

message(STATUS "Version: ${SLEIPNIR_VERSION}")

include_directories(${SLEIPNIR_INCLUDE_DIR})

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${SLEIPNIR_INSTALL_INCLUDE_DIR}
    COMPONENT sleipnir_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "InternalLoggers.hpp" EXCLUDE
)
