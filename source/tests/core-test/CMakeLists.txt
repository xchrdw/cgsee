
set(target core-test)
message(STATUS "Test ${target}")

#
# External libraries
#

find_package(GLM        REQUIRED)

#
# Includes
#

include_directories(
    ${GLM_INCLUDE_PATH}        
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/core/include
)

#
# Libraries
#

set(libs
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    core
)

#
# Sources
#

set(sources
    main.cpp
    aabb_test.cpp
)

#
# Build executable
#

add_executable(${target} ${sources})

target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")
