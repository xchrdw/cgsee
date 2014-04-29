
set(target core)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(GLEW       REQUIRED)
find_package(Assimp     REQUIRED)

find_package(Qt5Core    REQUIRED)
find_package(Qt5OpenGL  REQUIRED)
find_package(Qt5Widgets REQUIRED)

#
# Includes
#

include_directories(
    ${ASSIMP_INCLUDE_PATH}
    ${GLOW_INCLUDE_PATH}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${ASSIMP_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DCORE_STATIC")
else()
    add_definitions("-DCCORELIB_EXPORTS")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/core_api.h
)

set(sources

    ${source_path}/aabb.cpp
    ${source_path}/aabb.h
    ${source_path}/abstractapplication.cpp
    ${source_path}/abstractapplication.h
    ${source_path}/abstractmodelloader.cpp
    ${source_path}/abstractmodelloader.h
    ${source_path}/abstractstereocamera.h
    ${source_path}/abstractstereocamera.cpp
    ${source_path}/abstracttimer.h
    ${source_path}/assimploader.cpp
    ${source_path}/assimploader.h
    ${source_path}/autotimer.cpp
    ${source_path}/autotimer.h
    ${source_path}/bufferobject.cpp
    ${source_path}/bufferobject.h
    ${source_path}/camera.cpp
    ${source_path}/camera.h
    ${source_path}/cameraimplementation.cpp
    ${source_path}/cameraimplementation.h
    ${source_path}/convergentCamera.h
    ${source_path}/convergentCamera.cpp
    ${source_path}/chronotimer.cpp
    ${source_path}/chronotimer.h
    ${source_path}/common.h
    ${source_path}/coordinateprovider.cpp
    ${source_path}/coordinateprovider.h
    
    ${source_path}/fileassociatedshader.cpp
    ${source_path}/fileassociatedshader.h
    ${source_path}/framebufferobject.cpp
    ${source_path}/framebufferobject.h
    ${source_path}/geometryoptimizer.h
    ${source_path}/glformat.cpp
    ${source_path}/glformat.h
    ${source_path}/gpuquery.cpp
    ${source_path}/gpuquery.h
    ${source_path}/idpainter.cpp
    ${source_path}/idpainter.h
    ${source_path}/mathmacros.h
    ${source_path}/messagehandler.cpp
    ${source_path}/messagehandler.h
    ${source_path}/monocamera.h
    ${source_path}/monocamera.cpp
    ${source_path}/objloader.cpp
    ${source_path}/objloader.h
    ${source_path}/offAxisCamera.h
    ${source_path}/offAxisCamera.cpp
    ${source_path}/parallelCamera.h
    ${source_path}/parallelCamera.cpp
    ${source_path}/plane.cpp
    ${source_path}/plane.h
    ${source_path}/program.cpp
    ${source_path}/program.h
    ${source_path}/screenquad.cpp
    ${source_path}/screenquad.h
    ${source_path}/shader.cpp
    ${source_path}/shader.h
    ${source_path}/timer.cpp
    ${source_path}/timer.h
    ${source_path}/vertexcacheoptimizer.cpp
    ${source_path}/vertexcacheoptimizer.h
    ${source_path}/vertexreuse.cpp
    ${source_path}/vertexreuse.h
    ${source_path}/viewfrustum.cpp
    ${source_path}/viewfrustum.h

    ${source_path}/datacore/attributeiterator.h
    ${source_path}/datacore/attributeiterator_impl.inl
    ${source_path}/datacore/datablock.cpp
    ${source_path}/datacore/datablock.h
    ${source_path}/datacore/datablock_impl.inl
    ${source_path}/datacore/registertypeshere.h
    ${source_path}/datacore/typefunctions.cpp
    ${source_path}/datacore/typefunctions.h
    ${source_path}/datacore/vertexindexlist.cpp
    ${source_path}/datacore/vertexindexlist.h
    ${source_path}/datacore/vertexindexlist_impl.inl
    ${source_path}/datacore/vertexlist.cpp
    ${source_path}/datacore/vertexlist.h
    ${source_path}/datacore/vertexlist_impl.inl
    
    ${source_path}/navigation/abstractnavigation.cpp
    ${source_path}/navigation/abstractnavigation.h
    ${source_path}/navigation/arcballnavigation.cpp
    ${source_path}/navigation/arcballnavigation.h
    ${source_path}/navigation/flightnavigation.cpp
    ${source_path}/navigation/flightnavigation.h
    ${source_path}/navigation/fpsnavigation.cpp
    ${source_path}/navigation/fpsnavigation.h
    
    ${source_path}/painter/abstractglparent.h
    ${source_path}/painter/abstractpainter.h
    ${source_path}/painter/abstractpainter.cpp
    ${source_path}/painter/abstractscenepainter.h
    ${source_path}/painter/abstractscenepainter.cpp
    
    ${source_path}/property/abstractproperty.h
    ${source_path}/property/abstractproperty.cpp
    ${source_path}/property/valueproperty.h
    ${source_path}/property/limitedproperty.h
    ${source_path}/property/propertylist.h
    ${source_path}/property/propertylist.cpp
    ${source_path}/property/listproperty.h
    ${source_path}/property/listproperty.cpp
    ${source_path}/property/advancedlistproperty.h
    ${source_path}/property/advancedlistproperty.cpp
    ${source_path}/property/abstractpropertyvisitor.h
    ${source_path}/property/abstractpropertyvisitor.cpp
    ${source_path}/property/announcer.h
    ${source_path}/property/announcer.cpp

    ${source_path}/rendering/rendertechnique.h
    ${source_path}/rendering/rendertechnique.cpp
    ${source_path}/rendering/rasterizer.cpp
    ${source_path}/rendering/rasterizer.h
    ${source_path}/rendering/pathtracer.cpp
    ${source_path}/rendering/pathtracer.h
    ${source_path}/rendering/blureffect.cpp
    ${source_path}/rendering/blureffect.h
    ${source_path}/rendering/boundingboxpass.cpp
    ${source_path}/rendering/boundingboxpass.h
    ${source_path}/rendering/coloridpass.cpp
    ${source_path}/rendering/coloridpass.h   
    ${source_path}/rendering/defaultpass.cpp
    ${source_path}/rendering/defaultpass.h   
    ${source_path}/rendering/lightsource.cpp
    ${source_path}/rendering/lightsource.h
    ${source_path}/rendering/normalzpass.cpp
    ${source_path}/rendering/normalzpass.h   
    ${source_path}/rendering/renderingpass.cpp
    ${source_path}/rendering/renderingpass.h
    ${source_path}/rendering/shadowmapping.cpp
    ${source_path}/rendering/shadowmapping.h
    ${source_path}/rendering/ssaoeffect.cpp
    ${source_path}/rendering/ssaoeffect.h

    ${source_path}/scenegraph/absolutetransformator.cpp
    ${source_path}/scenegraph/absolutetransformator.h
    ${source_path}/scenegraph/cullingvisitor.cpp
    ${source_path}/scenegraph/cullingvisitor.h
    ${source_path}/scenegraph/defaultdrawmethod.cpp
    ${source_path}/scenegraph/defaultdrawmethod.h
    ${source_path}/scenegraph/drawmethod.h
    ${source_path}/scenegraph/drawvisitor.cpp
    ${source_path}/scenegraph/drawvisitor.h
    ${source_path}/scenegraph/group.cpp
    ${source_path}/scenegraph/group.h
    ${source_path}/scenegraph/highlightingdrawmethod.cpp
    ${source_path}/scenegraph/highlightingdrawmethod.h
    ${source_path}/scenegraph/node.cpp
    ${source_path}/scenegraph/node.h
    ${source_path}/scenegraph/pathtracingbvh.cpp
    ${source_path}/scenegraph/pathtracingbvh.h    
    ${source_path}/scenegraph/polygonaldrawable.cpp
    ${source_path}/scenegraph/polygonaldrawable.h
    ${source_path}/scenegraph/polygonalgeometry.cpp
    ${source_path}/scenegraph/polygonalgeometry.h
    ${source_path}/scenegraph/scenetraverser.cpp
    ${source_path}/scenegraph/scenetraverser.h
    ${source_path}/scenegraph/scenevisitorinterface.h
    ${source_path}/scenegraph/triangleobject.cpp
    ${source_path}/scenegraph/triangleobject.h
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
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})


add_definitions("-DGLM_SWIZZLE")
