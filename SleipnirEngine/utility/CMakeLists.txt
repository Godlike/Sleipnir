# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(SleipnirEngine_Utility)

add_library(${PROJECT_NAME} STATIC)
add_library(Sleipnir::Engine::Utility ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(SLEIPNIR_UTILITY_COMMON_SOURCES
    source/QSBR.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SLEIPNIR_UTILITY_COMMON_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Mule::Utilities
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${SLEIPNIR_INSTALL_INCLUDE_DIR}>
)

include(SleipnirInstallInfo)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${SLEIPNIR_INSTALL_INCLUDE_DIR}
    COMPONENT sleipnir_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "utility/InternalLoggers.hpp" EXCLUDE
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES FOLDER "${TARGET_FOLDER_ROOT}"
)
