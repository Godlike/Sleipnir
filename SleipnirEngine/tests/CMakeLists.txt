# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(${SLEIPNIR_ENGINE_NAME}Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CATCH_ROOT}/contrib")

include(ParseAndAddCatchTests)

## Build flags
include(SleipnirBuildFlags)

include_directories(${SLEIPNIR_INCLUDE_DIR} ${CATCH_ROOT}/single_include)

function(sleipnir_add_test)
    set(options SKIP_LINK)
    set(oneValueArgs NAME TIMEOUT)
    set(multiValueArgs SOURCE ARGS)
    cmake_parse_arguments(
        sleipnir_add_test
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}
    )

    set(EXEC_NAME "${sleipnir_add_test_NAME}Test")

    if (${SLEIPNIR_VERBOSE_TESTS})
        set(sleipnir_add_test_ARGS ${sleipnir_add_test_ARGS} -s)
    endif()

    set(sleipnir_add_test_ARGS ${sleipnir_add_test_ARGS} -o TestReport_${EXEC_NAME}.txt)

    if (NOT sleipnir_add_test_TIMEOUT)
        set(sleipnir_add_test_TIMEOUT 1)
    endif()

    add_executable(
        ${EXEC_NAME}
        ${sleipnir_add_test_SOURCE}
    )

    if (NOT sleipnir_add_test_SKIP_LINK)
        target_link_libraries(${EXEC_NAME}
            Sleipnir::Engine
        )
    else()
        target_include_directories(${EXEC_NAME}
            PUBLIC
                $<TARGET_PROPERTY:Sleipnir::Engine,INTERFACE_INCLUDE_DIRECTORIES>
        )
    endif()

    add_test(
        NAME ${sleipnir_add_test_NAME}
        COMMAND ${EXEC_NAME} ${sleipnir_add_test_ARGS}
    )

    set_tests_properties(
        ${sleipnir_add_test_NAME}
        PROPERTIES TIMEOUT ${sleipnir_add_test_TIMEOUT}
    )
endfunction()

sleipnir_add_test(NAME TemplateCounter
    SOURCE utility/TemplateCounterTest.cpp
    SKIP_LINK
)

sleipnir_add_test(NAME ChangeControl_TestUtils
    SOURCE
        utility/cc/ccTestUtilsTest.cpp
        utility/cc/ccTestUtils.hpp
        utility/cc/ccTestUtils.cpp
    SKIP_LINK
)

sleipnir_add_test(NAME ChangeControl_Changes
    SOURCE
        utility/cc/ChangesTest.cpp
        utility/cc/ccTestUtils.hpp
        utility/cc/ccTestUtils.cpp
    SKIP_LINK
)
