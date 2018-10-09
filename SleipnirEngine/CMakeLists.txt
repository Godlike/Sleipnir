# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(SleipnirEngine)

add_library(${PROJECT_NAME})
add_library(Sleipnir::Engine ALIAS ${PROJECT_NAME})

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

if (SLEIPNIR_BUILD_TESTS)
    enable_testing()
    include(CTest)
    set(CATCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Catch2)
    add_subdirectory(tests)
endif()

set(SLEIPNIR_ENTITY_SOURCES
    source/entity/Entity.cpp
    source/entity/EntityPool.cpp
    source/entity/World.cpp
)

set(SLEIPNIR_SYSTEM_SOURCES
    source/system/physics/BodyController.cpp
    source/system/physics/DynamicForceController.cpp
    source/system/physics/PegasusAdapter.cpp
    source/system/physics/Physics.cpp
    source/system/physics/PhysicsThread.cpp

    source/system/Lifetime.cpp
    source/system/Render.cpp
    source/system/Time.cpp
    source/system/Timer.cpp
)

set(SLEIPNIR_UTILITY_SOURCES
    source/utility/QSBR.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SLEIPNIR_ENTITY_SOURCES}
        ${SLEIPNIR_SYSTEM_SOURCES}
        ${SLEIPNIR_UTILITY_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${SLEIPNIR_INSTALL_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME}
    Pegasus::Physics
    Unicorn::Render
    Mule::Utilities
)
