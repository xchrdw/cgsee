
#include <core/scenegraph/defaultdrawmethod.h>

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/polygonalgeometry.h>


const QString DefaultDrawMethod::TRANSFORM_UNIFORM("transform");
const QString DefaultDrawMethod::ID_UNIFORM("u_id");
const QString DefaultDrawMethod::MATERIAL_UNIFORM("material");

DefaultDrawMethod::DefaultDrawMethod()
{
}

DefaultDrawMethod::~DefaultDrawMethod()
{
}

// void DefaultDrawMethod::draw(const Program & program, const glm::mat4 & transform, PolygonalGeometry & geometry, const gl::GLenum mode)
void DefaultDrawMethod::draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const
{
    PolygonalDrawable::t_geometryP geometry = drawable.geometry();
    
    if( geometry->indices().empty() )
        return;
    
    geometry->initialize(program);
    
    program.use();
    program.setUniform(TRANSFORM_UNIFORM, transform);
    program.setUniform(ID_UNIFORM, colorVectorFromId(drawable.id()));
    
    glm::mat4 materialCoeff;
    materialCoeff[0] = glm::vec4(0.1,0.1,0.1,1.0);    //ambient
    materialCoeff[1] = glm::vec4(1.0,1.0,1.0,1.0);    //diffuse
    materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);    //specular
    materialCoeff[3] = glm::vec4(0,0,0,0);            //emission
    program.setUniform(MATERIAL_UNIFORM, materialCoeff);
    
    gl::glBindVertexArray(geometry->vao());

    
    gl::glEnable(gl::GLenum::GL_DEPTH_TEST);
    //     gl::glEnable(gl::GLenum::GL_CULL_FACE);
    //     gl::glCullFace(gl::GLenum::GL_BACK);
    
    for( const auto & bo : geometry->elementArrayBOs() )
        bo->draw( drawable.mode() );
    
    gl::glDisable(gl::GLenum::GL_DEPTH_TEST);
    gl::glDisable(gl::GLenum::GL_CULL_FACE);
    
    gl::glBindVertexArray(0);

    
    program.release();
}

glm::vec4 DefaultDrawMethod::colorVectorFromId(const unsigned int id) const
{
    glm::vec4 id_vec;
    id_vec.r = float(id % 256)/255.0;
    id_vec.g = float((id >> 8) % 256)/255.0;
    id_vec.b = float((id >> 16) % 256)/255.0;
    id_vec.a = float((id >> 24) % 256)/255.0;
    
    return id_vec;
}
