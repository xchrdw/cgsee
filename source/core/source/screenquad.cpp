#include <core/screenquad.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Framebuffer.h>
#include <globjects/Program.h>

#include <core/bufferobject.h>
#include <core/gpuquery.h>


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
        glError();
    }
}

void ScreenQuad::initialize(globjects::Program & program) const
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
    glError();
    gl::glBindVertexArray(m_vao);                                                                  
    glError();

    // setup array buffer

    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(gl::GL_ARRAY_BUFFER, gl::GL_STATIC_DRAW);
	    m_vertexBO->data<gl::GLfloat>(vertices, 8, gl::GL_FLOAT, 2);
    }

    // bind all buffers to their attributes

    m_vertexBO->bind(program.getAttributeLocation("a_vertex"));

    gl::glBindVertexArray(0);
    glError();
}

void ScreenQuad::draw(
    globjects::Program & program
,   globjects::Framebuffer * target) const
{
    if(-1 == m_vao)
        initialize(program);

    if(target)
        target->bind(gl::GL_FRAMEBUFFER);

	glError();
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	glError();

    program.use();

    gl::glBindVertexArray(m_vao);                                                                  
    glError();

    gl::glDisable(gl::GL_DEPTH_TEST);
	glError();
    gl::glEnable(gl::GL_CULL_FACE);
	glError();
    gl::glDepthMask(gl::GL_FALSE);
	glError();

    gl::glDrawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    glError();

    gl::glDisable(gl::GL_CULL_FACE);
	glError();
    gl::glDepthMask(gl::GL_TRUE);
	glError();

    gl::glBindVertexArray(0);
    glError();
	
    if(target)
        target->unbind(gl::GL_FRAMEBUFFER);
}
