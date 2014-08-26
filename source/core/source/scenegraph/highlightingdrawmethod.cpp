
#include <core/scenegraph/highlightingdrawmethod.h>

#include <iostream>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Program.h>

#include <core/aabb.h>
#include <core/bufferobject.h>

#include <core/scenegraph/polygonalgeometry.h>
#include <core/scenegraph/polygonaldrawable.h>


HighlightingDrawMethod::HighlightingDrawMethod()
{
}

HighlightingDrawMethod::~HighlightingDrawMethod()
{
}

// void HighlightingDrawMethod::draw(const Program & program, const glm::mat4 & transform, PolygonalGeometry & geometry, const GLenum mode)
void HighlightingDrawMethod::draw(glo::Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const
{
    PolygonalDrawable::t_geometryP geometry = drawable.geometry();
    
    if( geometry->indices().empty() )
        return;
    
    geometry->initialize(program);
    
    program.use();
    program.setUniform(TRANSFORM_UNIFORM, transform);
    program.setUniform(ID_UNIFORM, colorVectorFromId(drawable.id()));
    
    glm::mat4 materialCoeff;
    materialCoeff[0] = glm::vec4(0.0,0.0,0.0,1.0); //ambient
    materialCoeff[1] = glm::vec4(.0,.0,.0,.0);    //diffuse
    materialCoeff[2] = glm::vec4(.0,.0,.0,.0);    //specular
    materialCoeff[3] = glm::vec4(1.0,0,0,0);      //emission  <---- zB nur die Emmission auf Rot.
    program.setUniform(MATERIAL_UNIFORM, materialCoeff);
    
    gl::glBindVertexArray(geometry->vao());
    glError();
    
    gl::glEnable(gl::GL_DEPTH_TEST);
    glError();
    //     glEnable(GL_CULL_FACE);
    //     glCullFace(GL_BACK);
    
    for( const auto & bo : geometry->elementArrayBOs() )
        bo->draw( drawable.mode() );
    
    gl::glDisable(gl::GL_DEPTH_TEST);
    glError();
    gl::glDisable(gl::GL_CULL_FACE);
    glError();
    
    gl::glBindVertexArray(0);
    glError();
    
    program.release();
}
