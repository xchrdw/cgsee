
set(target core)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(GLM        REQUIRED)
find_package(GLEW       REQUIRED)
find_package(OpenGL     REQUIRED)
find_package(ASSIMP     REQUIRED)
find_package(GLBINDING  REQUIRED)
find_package(GLOBJECTS  REQUIRED)

find_package(Qt5Core    REQUIRED)
find_package(Qt5OpenGL  REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(libzeug    REQUIRED)


#
# Includes
#

include_directories(
    ${ASSIMP_INCLUDE_DIRS}
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${GLOBJECTS_INCLUDES}
    ${LIBZEUG_INCLUDES}
	${GLBINDING_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
    ${GLOBJECTS_LIBRARIES}
    ${LIBZEUG_SIGNAL_LIBRARY}
	${GLBINDING_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DCORE_STATIC")
else()
    add_definitions("-DCORE_EXPORTS")
endif()

add_definitions("-DGLM_SWIZZLE")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/aabb.cpp
	${source_path}/abstractapplication.cpp
	${source_path}/abstractmodelloader.cpp
	${source_path}/assimploader.cpp
	${source_path}/autotimer.cpp
	${source_path}/bufferobject.cpp
	${source_path}/camera/abstractcamera.cpp
	${source_path}/camera/monocamera.cpp
	${source_path}/camera/projection.cpp
	${source_path}/camera/stereocamera.cpp
	${source_path}/chronotimer.cpp
	${source_path}/chronotimer.h
	${source_path}/coordinateprovider.cpp
	${source_path}/datacore/typefunctions.cpp
	${source_path}/datacore/datablock.cpp
	${source_path}/datacore/vertexindexlist.cpp
	${source_path}/datacore/vertexlist.cpp
	${source_path}/fileassociatedshader.cpp
	${source_path}/geometryoptimizer.h
	${source_path}/glformat.cpp
	${source_path}/gpuquery.cpp
	${source_path}/idpainter.cpp
	${source_path}/idpainter.h
	${source_path}/material/material.cpp
	${source_path}/material/materialdictionary.cpp
	${source_path}/messagehandler.cpp
	${source_path}/navigation/flightnavigation.cpp
	${source_path}/navigation/fpsnavigation.cpp
	${source_path}/navigation/abstractnavigation.cpp
	${source_path}/navigation/arcballnavigation.cpp
	${source_path}/navigation/navigationhistory.cpp
	${source_path}/navigation/navigationhistoryelement.cpp
	${source_path}/objloader.cpp
	${source_path}/painter/abstractscenepainter.cpp
	${source_path}/painter/abstractpainter.cpp
    ${source_path}/painter/pipelinepainter.cpp
	${source_path}/plane.cpp
	${source_path}/program.cpp
	${source_path}/property/abstractproperty.cpp
	${source_path}/property/abstractpropertyvisitor.cpp
	${source_path}/property/advancedlistproperty.cpp
	${source_path}/property/listproperty.cpp
	${source_path}/property/announcer.cpp
	${source_path}/property/propertylist.cpp
    ${source_path}/renderbufferobject.cpp
	${source_path}/rendering/blureffect.cpp
	${source_path}/rendering/boundingboxpass.cpp
	${source_path}/rendering/renderingpass.cpp
    ${source_path}/rendering/renderstage.cpp
	${source_path}/rendering/abstractpostprocessingstage.cpp
    ${source_path}/rendering/ssaostage.cpp
	${source_path}/rendering/abstractscenerenderstage.cpp
    ${source_path}/rendering/abstractrenderstage.cpp
	${source_path}/rendering/lightingsystem.cpp
	${source_path}/rendering/lightsource.cpp
	${source_path}/rendering/rasterizer.cpp
	${source_path}/rendering/normalzpass.cpp
	${source_path}/rendering/ssaoeffect.cpp
	${source_path}/rendering/coloridpass.cpp
	${source_path}/rendering/shadowmapping.cpp
	${source_path}/rendering/defaultpass.cpp
	${source_path}/rendering/rendertechnique.cpp
	${source_path}/rendering/pathtracer.cpp
    ${source_path}/rendering/pipelinebuilder.cpp
	${source_path}/rendering/rasterizationpipelinebuilder.cpp
	${source_path}/rendering/pathtracingpipelinebuilder.cpp
	${source_path}/scenegraph/scenetraverser.cpp
	${source_path}/scenegraph/node.cpp
	${source_path}/scenegraph/drawvisitor.cpp
	${source_path}/scenegraph/polygonalgeometry.cpp
	${source_path}/scenegraph/polygonaldrawable.cpp
	${source_path}/scenegraph/group.cpp
	${source_path}/scenegraph/defaultdrawmethod.cpp
	${source_path}/scenegraph/cullingvisitor.cpp
	${source_path}/scenegraph/triangleobject.cpp
	${source_path}/scenegraph/highlightingdrawmethod.cpp
	${source_path}/scenegraph/pathtracingbvh.cpp
	${source_path}/scenegraph/absolutetransformator.cpp
	${source_path}/screenquad.cpp
	${source_path}/shader.cpp
	${source_path}/timer.cpp
	${source_path}/vertexcacheoptimizer.cpp
	${source_path}/vertexcacheoptimizer.h
	${source_path}/vertexreuse.cpp
	${source_path}/vertexreuse.h
	${source_path}/viewfrustum.cpp
)

set(api_includes
	${include_path}/aabb.h
	${include_path}/abstractapplication.h
	${include_path}/abstracteventhandler.h
	${include_path}/abstractmodelloader.h
	${include_path}/abstracttimer.h
	${include_path}/assimploader.h
	${include_path}/autotimer.h
	${include_path}/bufferobject.h
	${include_path}/camera/abstractcamera.h
	${include_path}/camera/monocamera.h
	${include_path}/camera/projection.h
	${include_path}/camera/stereocamera.h
	${include_path}/coordinateprovider.h
	${include_path}/core_api.h
	${include_path}/datacore/datablock.h
	${include_path}/datacore/vertexlist.h
	${include_path}/datacore/typefunctions.h
	${include_path}/datacore/registertypeshere.h
	${include_path}/datacore/vertexindexlist.h
	${include_path}/datacore/vertexlist_impl.tli
	${include_path}/datacore/datablock_impl.tli
	${include_path}/datacore/attributeiterator_impl.tli
	${include_path}/datacore/vertexindexlist_impl.tli
	${include_path}/datacore/attributeiterator.h
	${include_path}/declspec.h
	${include_path}/fileassociatedshader.h
	${include_path}/glformat.h
	${include_path}/gpuquery.h
	${include_path}/material/material.h
	${include_path}/material/materialdictionary.h
	${include_path}/mathmacros.h
	${include_path}/messagehandler.h
	${include_path}/navigation/fpsnavigation.h
	${include_path}/navigation/abstractnavigation.h
	${include_path}/navigation/flightnavigation.h
	${include_path}/navigation/arcballnavigation.h
	${include_path}/navigation/navigationhistory.h
	${include_path}/navigation/navigationhistoryelement.h
	${include_path}/objloader.h
	${include_path}/painter/abstractscenepainter.h
	${include_path}/painter/abstractpainter.h
    ${include_path}/painter/pipelinepainter.h
	${include_path}/plane.h
	${include_path}/program.h
	${include_path}/property/limitedproperty.h
	${include_path}/property/abstractpropertyvisitor.h
	${include_path}/property/announcer.h
	${include_path}/property/listproperty.h
	${include_path}/property/valueproperty.h
	${include_path}/property/advancedlistproperty.h
	${include_path}/property/abstractproperty.h
	${include_path}/property/propertylist.h
    ${include_path}/renderbufferobject.h
	${include_path}/rendering/rendertechnique.h
    ${include_path}/rendering/renderstage.h
	${include_path}/rendering/abstractpostprocessingstage.h
    ${include_path}/rendering/ssaostage.h
	${include_path}/rendering/abstractscenerenderstage.h
    ${include_path}/rendering/abstractrenderstage.h
	${include_path}/rendering/shadowmapping.h
	${include_path}/rendering/ssaoeffect.h
	${include_path}/rendering/normalzpass.h
	${include_path}/rendering/coloridpass.h
	${include_path}/rendering/blureffect.h
	${include_path}/rendering/lightingsystem.h
	${include_path}/rendering/lighttypes.h
	${include_path}/rendering/lightsource.h
	${include_path}/rendering/rasterizer.h
	${include_path}/rendering/defaultpass.h
	${include_path}/rendering/boundingboxpass.h
	${include_path}/rendering/renderingpass.h
	${include_path}/rendering/pathtracer.h
    ${include_path}/rendering/pipelinebuilder.h
	${include_path}/rendering/rasterizationpipelinebuilder.h
	${include_path}/rendering/pathtracingpipelinebuilder.h
	${include_path}/scenegraph/group.h
	${include_path}/scenegraph/highlightingdrawmethod.h
	${include_path}/scenegraph/triangleobject.h
	${include_path}/scenegraph/defaultdrawmethod.h
	${include_path}/scenegraph/node.h
	${include_path}/scenegraph/scenevisitorinterface.h
	${include_path}/scenegraph/polygonalgeometry.h
	${include_path}/scenegraph/drawvisitor.h
	${include_path}/scenegraph/drawmethod.h
	${include_path}/scenegraph/polygonaldrawable.h
	${include_path}/scenegraph/scenetraverser.h
	${include_path}/scenegraph/pathtracingbvh.h
	${include_path}/scenegraph/absolutetransformator.h
	${include_path}/scenegraph/cullingvisitor.h
	${include_path}/screenquad.h
	${include_path}/shader.h
	${include_path}/timer.h
	${include_path}/typedefs.h
	${include_path}/viewfrustum.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$|\\\\.tli"
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.tli"
    ${source_group} ${sources})

#
# Build library
#

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()

qt5_use_modules(${target} Widgets OpenGL)
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
