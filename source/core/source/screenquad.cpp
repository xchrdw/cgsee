
#include <core/screenquad.h>

#include <core/bufferobject.h>
#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/shader.h>
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

        glDeleteVertexArrays(1, &m_vao);
        glError();
    }
}

void ScreenQuad::initialize(const Program & program) const
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const GLfloat vertices[] =
    {
        +1.f, -1.f
    ,   +1.f, +1.f
    ,   -1.f, -1.f
    ,   -1.f, +1.f
    };

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);                                                                  
    glError();

    // setup array buffer

    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	    m_vertexBO->data<GLfloat>(vertices, 8, GL_FLOAT, 2);
    }

    // bind all buffers to their attributes

    m_vertexBO->bind(program.attributeLocation("a_vertex"));

    glBindVertexArray(0);
    glError();
}

void ScreenQuad::draw(const Program & program) const
{
    if(-1 == m_vao)
        initialize(program);

    glClear(GL_COLOR_BUFFER_BIT);

    program.use();

    glBindVertexArray(m_vao);                                                                  
    glError();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glError();

    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glBindVertexArray(0);
    glError();
}
