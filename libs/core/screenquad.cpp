
#include "screenquad.h"

#include "framebufferobject.h"
#include "program.h"
#include "shader.h"
#include "gpuquery.h"


ScreenQuad::ScreenQuad()
:   m_buffer(-1)
,   m_vao(-1)
,   m_program(nullptr)
{
} 

ScreenQuad::~ScreenQuad()
{
    if(isBuffer())
    {
        glDeleteVertexArrays(1, &m_vao);
        glError();

        glDeleteBuffers(1, &m_buffer);
        glError();
    }

    if(m_program)
        delete m_program;
}

inline const bool ScreenQuad::isBuffer() const
{
    return m_buffer != -1;
}

void ScreenQuad::initialize() const
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    const GLfloat vertices[] =
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

    glGenBuffers(1, &m_buffer);
    glError();
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glError();
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glError();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glError();

    glBindVertexArray(0);
	glError();
}

void ScreenQuad::initializeDefaultProgram() const
{
    if(m_program)
        return;

    m_program = new Program();
    
    Shader * v = new Shader(GL_VERTEX_SHADER);
    Shader * f = new Shader(GL_FRAGMENT_SHADER);

    v->setSource(
        "#version 150 core\n"
        "\n"
        "in vec3 a_vertex;\n"
        "out vec2 v_uv;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "	v_uv = a_vertex.xy * 0.5 + 0.5;\n"
        "	gl_Position = vec4(a_vertex, 1.0);\n"
        "}\n");

    f->setSource(
        "#version 150 core\n"
        "\n"
        "in vec2 v_uv;\n"
        "out vec4 fragcolor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    fragcolor = vec4(0.3, 0.4, 0.5, 1.0);\n"
        "}\n");

    m_program->attach(v);
    m_program->attach(f);

    m_vertex = m_program->attributeLocation("a_vertex");
}

void ScreenQuad::draw(FrameBufferObject * target) const
{
    if(!m_program)
        initializeDefaultProgram();

    draw(*m_program, target);
}

void ScreenQuad::draw(
    const Program & program
,   FrameBufferObject * target) const
{
    if(!isBuffer())
        initialize();

    program.use();

    const int a_vertex = (&program == m_program) ? 
        m_vertex : program.attributeLocation("a_vertex");

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glBindVertexArray(m_vao);                                                                  
	glError();

    glEnableVertexAttribArray(a_vertex);
    glError();
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glError();
    glVertexAttribPointer(a_vertex, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glError();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glError();

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glError();
    glBindVertexArray(0);
	glError();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    if(target)
        target->release();

    m_program->release();
}

void ScreenQuad::draw(
    const GLuint program
,   const GLint vertexAttributeLocation
,   FrameBufferObject * target) const
{
    if(program == -1)
    {
        qCritical("No valid program available for rendering screen aligned quad.");
        return;
    }

    if(!isBuffer())
        initialize();

    glUseProgram(program);
    glError();

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glError();
    glVertexAttribPointer(vertexAttributeLocation, 2, GL_FLOAT, 0, 0, 0);
    glError();
    glEnableVertexAttribArray(vertexAttributeLocation);
    glError();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glError();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    if(target)
        target->release();

    glUseProgram(0);
    glError();
}