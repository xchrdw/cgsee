#include <GL/glew.h>

#include <iostream>

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "polygonaldrawable.h"
#include "polygonalgeometry.h"
#include "highlightingdrawmethod.h"


HighlightingDrawMethod::HighlightingDrawMethod()
{
}

HighlightingDrawMethod::~HighlightingDrawMethod()
{
}

// void HighlightingDrawMethod::draw(const Program & program, const glm::mat4 & transform, PolygonalGeometry & geometry, const GLenum mode)
void HighlightingDrawMethod::draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const
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
    
    glBindVertexArray(geometry->vao());
    glError();
    
    glEnable(GL_DEPTH_TEST);
    //     glEnable(GL_CULL_FACE);
    //     glCullFace(GL_BACK);
    
    for( const auto & bo : geometry->elementArrayBOs() )
        bo->draw( drawable.mode() );
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glBindVertexArray(0);
    glError();
    
    program.release();
}
