# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(SLEIPNIR_ENGINE_NAME "SleipnirEngine" CACHE STRING "Sleipnir project name.")

if(NOT DEFINED SLEIPNIR_ROOT)
    set(SLEIPNIR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Sleipnir root directory.")
endif()

if(NOT DEFINED SLEIPNIR_ENGINE_ROOT)
    set(SLEIPNIR_ENGINE_ROOT "${SLEIPNIR_ROOT}/SleipnirEngine" CACHE STRING "Sleipnir Engine root directory.")
endif()

list(APPEND CMAKE_MODULE_PATH "${SLEIPNIR_ENGINE_ROOT}/cmake")

set(SLEIPNIR_INCLUDE_DIR
    ${SLEIPNIR_ENGINE_ROOT}/include
    CACHE LIST "Sleipnir include directories."
)

if (NOT DEFINED INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include/godlike" CACHE STRING "Path to directory holding headers")
endif()

if (NOT DEFINED INSTALL_LIBRARY_DIR)
    set(INSTALL_LIBRARY_DIR "lib" CACHE STRING "Path to directory holding libraries")
endif()

set(SLEIPNIR_ENGINE_LIB ${SLEIPNIR_ENGINE_NAME} CACHE STRING "Name of Sleipnir Engine library")
