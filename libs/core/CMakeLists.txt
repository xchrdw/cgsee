
set(target core)
message(STATUS "lib ${target}")

configure_file(meta.template meta.h)

set(SOURCES

    declspec.h
    
    aabb.cpp
    aabb.h
    abstractapplication.cpp
    abstractapplication.h
    abstractmodelloader.cpp
    abstractmodelloader.h
    abstractStereoCamera.h
    abstractStereoCamera.cpp
    abstracttimer.h
    assimploader.cpp
    assimploader.h
    autotimer.cpp
    autotimer.h
    bufferobject.cpp
    bufferobject.h
    camera.cpp
    camera.h
    cameraimplementation.cpp
    cameraimplementation.h
    convergentCamera.h
    convergentCamera.cpp
    chronotimer.cpp
    chronotimer.h
    common.h
    coordinateprovider.cpp
    coordinateprovider.h
	
    fileassociatedshader.cpp
    fileassociatedshader.h
    framebufferobject.cpp
    framebufferobject.h
    geometryoptimizer.h
    glformat.cpp
    glformat.h
    gpuquery.cpp
    gpuquery.h
    idpainter.cpp
    idpainter.h
    mathmacros.h
    messagehandler.cpp
    messagehandler.h
    objloader.cpp
    objloader.h
    offAxisCamera.h
    offAxisCamera.cpp
    pathtracer.cpp
    pathtracer.h
    parallelCamera.h
    parallelCamera.cpp
    plane.cpp
    plane.h
    program.cpp
    program.h
    rasterizingcamera.cpp
    rasterizingcamera.h
    screenquad.cpp
    screenquad.h
    shader.cpp
    shader.h
    timer.cpp
    timer.h
    vertexcacheoptimizer.cpp
    vertexcacheoptimizer.h
    vertexreuse.cpp
    vertexreuse.h
    viewfrustum.cpp
    viewfrustum.h

    datacore/attributeiterator.h
    datacore/attributeiterator_impl.inl
    datacore/datablock.cpp
    datacore/datablock.h
    datacore/datablock_impl.inl
    datacore/registertypeshere.h
    datacore/typefunctions.cpp
    datacore/typefunctions.h
    datacore/vertexindexlist.cpp
    datacore/vertexindexlist.h
    datacore/vertexindexlist_impl.inl
    datacore/vertexlist.cpp
    datacore/vertexlist.h
    datacore/vertexlist_impl.inl
	
    navigation/abstractnavigation.cpp
    navigation/abstractnavigation.h
    navigation/arcballnavigation.cpp
    navigation/arcballnavigation.h
    navigation/flightnavigation.cpp
    navigation/flightnavigation.h
    navigation/fpsnavigation.cpp
    navigation/fpsnavigation.h
    
    painter/abstractglparent.h
    painter/abstractpainter.h
    painter/abstractpainter.cpp
    painter/abstractscenepainter.h
    painter/abstractscenepainter.cpp
    
    property/abstractproperty.h
    property/abstractproperty.cpp
    property/valueproperty.h
    property/limitedproperty.h
    property/propertylist.h
    property/propertylist.cpp
    property/listproperty.h
    property/listproperty.cpp
    property/advancedlistproperty.h
    property/advancedlistproperty.cpp
    property/abstractpropertyvisitor.h
    property/abstractpropertyvisitor.cpp
    property/announcer.h
    property/announcer.cpp
	
    rendering/blureffect.cpp
    rendering/blureffect.h
    rendering/boundingboxpass.cpp
    rendering/boundingboxpass.h
    rendering/coloridpass.cpp
    rendering/coloridpass.h   
    rendering/defaultpass.cpp
    rendering/defaultpass.h   
    rendering/lightsource.cpp
    rendering/lightsource.h
    rendering/normalzpass.cpp
    rendering/normalzpass.h   
    rendering/renderingpass.cpp
    rendering/renderingpass.h
    rendering/shadowmapping.cpp
    rendering/shadowmapping.h
    rendering/ssaoeffect.cpp
    rendering/ssaoeffect.h

    scenegraph/absolutetransformator.cpp
    scenegraph/absolutetransformator.h
    scenegraph/cullingvisitor.cpp
    scenegraph/cullingvisitor.h
    scenegraph/defaultdrawmethod.cpp
    scenegraph/defaultdrawmethod.h
    scenegraph/drawmethod.h
    scenegraph/drawvisitor.cpp
    scenegraph/drawvisitor.h
    scenegraph/group.cpp
    scenegraph/group.h
    scenegraph/highlightingdrawmethod.cpp
    scenegraph/highlightingdrawmethod.h
    scenegraph/node.cpp
    scenegraph/node.h
    scenegraph/pathtracingbvh.cpp
    scenegraph/pathtracingbvh.h    
    scenegraph/polygonaldrawable.cpp
    scenegraph/polygonaldrawable.h
    scenegraph/polygonalgeometry.cpp
    scenegraph/polygonalgeometry.h
    scenegraph/scenetraverser.cpp
    scenegraph/scenetraverser.h
    scenegraph/scenevisitorinterface.h
    scenegraph/triangleobject.cpp
    scenegraph/triangleobject.h
)

source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.ui$|\\\\.inl$" 
    ${CGSEE_SOURCE_GROUP} ${SOURCES})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})

add_library(${target} SHARED ${HEADERS} ${SOURCES})

qt5_use_modules(${target} Widgets OpenGL)

target_link_libraries(${target}
    ${ASSIMP_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${EXTRA_LIBS}
)


set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    FOLDER ${CGSEE_LIB_FOLDER}
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")

add_definitions("-DCGSEE_EXPORTS")
add_definitions("-DGLM_SWIZZLE")
