# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(SleipnirEngine)

set(SLEIPNIR_LIB_ALIAS Sleipnir::Engine)

add_library(${PROJECT_NAME} "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(${SLEIPNIR_LIB_ALIAS} ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

option(SLEIPNIR_BUILD_DOCUMENTATION "Build Sleipnir documentation" OFF)
option(SLEIPNIR_BUILD_DEMOS "Build demo projects" OFF)
option(SLEIPNIR_BUILD_TESTS "Build Sleipnir tests" ON)

message(STATUS "${PROJECT_NAME} ${SLEIPNIR_VERSION} ${CMAKE_BUILD_TYPE} configuration:")
message(STATUS "-- SLEIPNIR_BUILD_DOCUMENTATION: ${SLEIPNIR_BUILD_DOCUMENTATION}")
message(STATUS "-- SLEIPNIR_BUILD_DEMOS: ${SLEIPNIR_BUILD_DEMOS}")
message(STATUS "-- SLEIPNIR_BUILD_TESTS: ${SLEIPNIR_BUILD_TESTS}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#Build flags
if (UNIX)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-command-line-argument")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W3")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(TARGET_FOLDER_ROOT "sleipnir")

add_subdirectory(ecs)
add_subdirectory(utility)

if (SLEIPNIR_BUILD_TESTS)
    enable_testing()
    include(CTest)
    set(CATCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Catch2)
    add_subdirectory(tests)
endif()

set(SLEIPNIR_COMMON_SOURCES
    source/Loggers.cpp
    source/SleipnirEngine.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SLEIPNIR_COMMON_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${SLEIPNIR_INSTALL_INCLUDE_DIR}>
        $<TARGET_PROPERTY:Sleipnir::Engine::ECS,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Sleipnir::Engine::Utility,INTERFACE_INCLUDE_DIRECTORIES>
)

# external depencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Mule::Utilities
        Tulpar::Audio
)

# internal static libraries
if (UNIX)
    set(EXPORT_SYMBOLS_PREFIX "-Wl,--whole-archive")
    set(EXPORT_SYMBOLS_SUFFIX "-Wl,--no-whole-archive")
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${EXPORT_SYMBOLS_PREFIX}
        Sleipnir::Engine::ECS
        Sleipnir::Engine::Utility
        ${EXPORT_SYMBOLS_SUFFIX}
)

include(SleipnirInstallInfo)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${SLEIPNIR_VERSION}
        SOVERSION ${SLEIPNIR_SOVERSION}
)

install( TARGETS ${PROJECT_NAME}
    COMPONENT sleipnir_libraries
    RUNTIME DESTINATION ${SLEIPNIR_INSTALL_LIBRARY_DIR}
    LIBRARY DESTINATION ${SLEIPNIR_INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${SLEIPNIR_INSTALL_LIBRARY_DIR}
)

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

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${SLEIPNIR_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.gen

    @ONLY
)

file(GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.gen
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${SLEIPNIR_INSTALL_LIBRARY_DIR}
    COMPONENT sleipnir_headers
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES FOLDER "${TARGET_FOLDER_ROOT}"
)
