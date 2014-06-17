
set(target cgsee)
message(STATUS "App ${target}")

#
# External libraries
#

find_package(GLM        REQUIRED)
find_package(GLEW       REQUIRED)
find_package(GLOW       REQUIRED)

#
# Includes
#

include_directories(
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${GLOW_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/core/include
    ${CMAKE_SOURCE_DIR}/source/gui/include
)

#
# Libraries
#

set(libs
    ${GLOW_LIBRARIES}
    core
    gui
)

#
# Sources
#

set(sources
    cgsee.h
    cgsee.cpp
    main.cpp
    painter.h
    painter.cpp
)

#
# Build executable
#

add_executable(${target} ${sources})

qt5_use_modules(${target} Core OpenGL Gui Widgets)

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

#
# Deployment
#

# deploy qt binaries (cgsee debug target should not be deployed)
if(WIN32)
    install_qt(${INSTALL_BIN} Core Gui Widgets OpenGL)
endif()

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
