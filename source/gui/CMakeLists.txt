
set(target gui)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(GLM        REQUIRED)
find_package(GLEW       REQUIRED)
find_package(OpenGL     REQUIRED)
find_package(ASSIMP     REQUIRED)
find_package(GLOBJECTS  REQUIRED)

find_package(Qt5Core    REQUIRED)
find_package(Qt5Gui     REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(libzeug    REQUIRED)

#
# Includes
#

include_directories(
    ${ASSIMP_INCLUDE_DIRS}
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${LIBZEUG_INCLUDES}
    ${GLOBJECTS_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/source/core/include
)

#
# Libraries
#

set(libs
    ${ASSIMP_LIBRARIES}
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    ${LIBZEUG_REFLECTION_LIBRARY}
    ${LIBZEUG_PROPERTYGUI_LIBRARY}
    ${LIBZEUG_SIGNAL_LIBRARY}
    ${GLOBJECTS_LIBRARIES}
    core
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGUI_STATIC")
else()
    add_definitions("-DGUI_EXPORTS")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/canvas.cpp
	${source_path}/canvasexportconfig.cpp
	${source_path}/canvasexportconfig.h
	${source_path}/canvasexporter.cpp
	${source_path}/canvasexporter.ui
	${source_path}/fileExplorer.cpp
	${source_path}/fileNavigator.cpp
	${source_path}/propertywidgetbuilder.cpp
    ${source_path}/materialwidgetbuilder.cpp
	${source_path}/viewer.cpp
	${source_path}/viewer.ui
)

set(api_includes
	${include_path}/canvasexporter.h
	${include_path}/canvas.h
	${include_path}/fileExplorer.h
	${include_path}/fileNavigator.h
	${include_path}/gui_api.h
	${include_path}/propertywidgetbuilder.h
    ${include_path}/materialwidgetbuilder.h
    ${include_path}/viewer.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

list_extract(uis "\\\\.ui$" ${sources})
qt5_wrap_ui(uihs ${uis})

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()

qt5_use_modules(${target} Core Gui Widgets OpenGL)
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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH                ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
