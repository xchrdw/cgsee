
#include <core/screenquad.h>

#include <glbinding/glbinding.h>
#include <globjects/globjects.h>

#include <core/bufferobject.h>
#include <core/program.h>
#include <core/shader.h>


ScreenQuad::ScreenQuad()
:   m_vao(-1)
,   m_vertexBO(nullptr)
{
}

ScreenQuad::~ScreenQuad()
{
    if(-1 != m_vao)
    {
        delete m_vertexBO;

        gl::glDeleteVertexArrays(1, &m_vao);
    
    }
}

void ScreenQuad::initialize(const Program & program) const
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const gl::GLfloat vertices[] =
    {
        +1.f, -1.f
    ,   +1.f, +1.f
    ,   -1.f, -1.f
    ,   -1.f, +1.f
    };

    gl::glGenVertexArrays(1, &m_vao);

    gl::glBindVertexArray(m_vao);                                                                  


    // setup array buffer

    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(gl::GLenum::GL_ARRAY_BUFFER, gl::GLenum::GL_STATIC_DRAW);
	    m_vertexBO->data<gl::GLfloat>(vertices, 8, gl::GLenum::GL_FLOAT, 2);
    }

    // bind all buffers to their attributes

    m_vertexBO->bind(program.attributeLocation("a_vertex"));

    gl::glBindVertexArray(0);

}

void ScreenQuad::draw(const Program & program) const
{
    if(-1 == m_vao)
        initialize(program);

    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    program.use();

    gl::glBindVertexArray(m_vao);                                                                  


    gl::glDisable(gl::GLenum::GL_DEPTH_TEST);
    gl::glEnable(gl::GLenum::GL_CULL_FACE);
    gl::glDepthMask(gl::GL_FALSE);

    gl::glDrawArrays(gl::GLenum::GL_TRIANGLE_STRIP, 0, 4);


    gl::glDisable(gl::GLenum::GL_CULL_FACE);
    gl::glDepthMask(gl::GL_TRUE);

    gl::glBindVertexArray(0);

}
