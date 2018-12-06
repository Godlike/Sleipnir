# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(SleipnirEngine_ECS)

add_library(${PROJECT_NAME} STATIC)
add_library(Sleipnir::Engine::ECS ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(SLEIPNIR_ECS_ENTITY_SOURCES
    source/entity/Entity.cpp
    source/entity/EntityPool.cpp
    source/entity/World.cpp
)

set(SLEIPNIR_ECS_SYSTEM_SOURCES
    source/system/physics/BodyController.cpp
    source/system/physics/DynamicForceController.cpp
    source/system/physics/PegasusAdapter.cpp
    source/system/physics/Physics.cpp
    source/system/physics/PhysicsThread.cpp

    source/system/Lifetime.cpp
    source/system/Render.cpp
    source/system/TimeBase.cpp
    source/system/Timer.cpp
    source/system/TimeWithPhysics.cpp
)

set(SLEIPNIR_ECS_COMMON_SOURCES
    source/Systems.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SLEIPNIR_ECS_ENTITY_SOURCES}
        ${SLEIPNIR_ECS_SYSTEM_SOURCES}
        ${SLEIPNIR_ECS_COMMON_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Sleipnir::Engine::Utility
        Pegasus::Physics
        Unicorn::Render
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
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES FOLDER "${TARGET_FOLDER_ROOT}"
)
