#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "painter.h"

#include <GL/glew.h>

#include <QGLWidget>
#include <QMatrix4x4>

#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/gpuquery.h>
#include <core/timer.h>


Painter::Painter()
:   AbstractPainter()

,    m_height(1)
,    m_width (1)
,    m_aspect(1.f)

,    m_model(1.f)
,    m_view(1.f)
,    m_projection(1.f)

,    m_eye   (glm::vec3(0.f, 0.f,-1.5))
,    m_center(glm::vec3(0.f, 0.f, 0.f))
,    m_up    (glm::vec3(0.f, 1.f, 0.f))

,    m_fovy  (45.f)
,    m_zNear ( .1f)
,    m_zFar  (10.f)

,    m_vert(-1)
,    m_frag(-1)
,    m_program(-1)

,    a_vertex(-1)
,    u_transform(-1)
,    u_texture(-1)
,    u_clicked(-1)
,    a_texCoord(-1)

,    m_vao(-1)
,    m_indices(-1)
,    m_vertices(-1)
,    m_texCoords(-1)
,    m_textureID(0)

,    m_clockwise(1)
,    m_clicked(false)
{
}

Painter::~Painter()
{
    glDeleteBuffers(1, &m_vertices);
    glDeleteBuffers(1, &m_texCoords);
    glDeleteBuffers(1, &m_indices);

    glDetachShader(m_program, m_vert);
    glDetachShader(m_program, m_frag);

    glDeleteShader(m_vert);
    glDeleteShader(m_frag);

    glDeleteTextures(1, &m_textureID);

    glDeleteProgram(m_program);
}

void getCompileInfo(const GLint shader)
{
    GLint status(GL_FALSE);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    const GLboolean compiled = (GL_TRUE == status);
    GLchar * log = "";

    if(!compiled)
    {
        GLint maxLength(0);
        GLint logLength(0);

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar *chr = new GLchar[maxLength];
        glGetShaderInfoLog(shader, maxLength, &logLength, chr);

        log = chr;

        qCritical("Compiling shader failed.");
        qCritical("%s", log);
    }
}


const bool Painter::initialize()
{
    // Initialize view
    m_view = glm::lookAt(m_eye, m_center, m_up);

    // Initialize shaders and program
    const QString TEXTURE_FILENAME = "data/minion.png";
    m_textureID = loadTexture(TEXTURE_FILENAME);
    m_vert = glCreateShader(GL_VERTEX_SHADER);
    m_frag = glCreateShader(GL_FRAGMENT_SHADER);

    // Vertex shader
    static const char * srcv =
        "#version 150 core\n"
        "\n"
        "uniform mat4 transform;\n"
        "in vec3 a_vertex;\n"
        "in vec2 vertTexCoord;\n"
        "out vec2 fragTexCoord;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    vec4 vertex = transform * vec4(a_vertex, 1.f);\n"
        "    fragTexCoord = vertTexCoord;\n"
        "    gl_Position = vertex;\n"
        "}\n";

    // Fragement shader
    static const char * srcf =
        "#version 150 core\n"
        "\n"
        "uniform sampler2D tex;\n"
        "uniform float clicked;\n"
        "in vec2 fragTexCoord;\n"
        "out vec4 fragcolor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec3 texRgb = texture(tex, fragTexCoord).xyz;\n"
        "    float x = clamp(texRgb.x + clicked, 0.f, 1.f);\n"
        "    float y = clamp(texRgb.y + clicked, 0.f, 1.f);\n"
        "    float z = clamp(texRgb.z + clicked, 0.f, 1.f);\n"
        "    fragcolor = vec4(x, y, z, 1.f);\n"
        "}\n";

    glShaderSource(m_vert, 1, &srcv, nullptr);
    glCompileShader(m_vert);
    getCompileInfo(m_vert);

    glShaderSource(m_frag, 1, &srcf, nullptr);
    glCompileShader(m_frag);
    getCompileInfo(m_frag);

    m_program = glCreateProgram();

    glAttachShader(m_program, m_vert);
    glAttachShader(m_program, m_frag);
    glLinkProgram(m_program);

    a_vertex = glGetAttribLocation(m_program, "a_vertex");
    u_transform = glGetUniformLocation(m_program, "transform");
    u_clicked = glGetUniformLocation(m_program, "clicked");
    a_texCoord = glGetAttribLocation(m_program, "vertTexCoord");

    // Initialize geometry: 6 faces * 4 vertices
    static const GLfloat vertices[72] =
    {
        // front        // v // ii
        -1.f,-1.f, 1.f, // 1 // 00
         1.f,-1.f, 1.f, // 5 // 01
         1.f, 1.f, 1.f, // 7 // 02
        -1.f, 1.f, 1.f, // 3 // 03

        // back         // v // ii
        -1.f,-1.f,-1.f, // 0 // 04
         1.f,-1.f,-1.f, // 4 // 05
         1.f, 1.f,-1.f, // 6 // 06
        -1.f, 1.f,-1.f, // 2 // 07

        // left         // v // ii
        -1.f,-1.f, 1.f, // 1 // 08
        -1.f,-1.f,-1.f, // 0 // 09
        -1.f, 1.f,-1.f, // 2 // 10
        -1.f, 1.f, 1.f, // 3 // 11

        // right        // v // ii
         1.f,-1.f, 1.f, // 5 // 12
         1.f,-1.f,-1.f, // 4 // 13
         1.f, 1.f,-1.f, // 6 // 14
         1.f, 1.f, 1.f, // 7 // 15

        // top          // v // ii
        -1.f, 1.f, 1.f, // 3 // 16
         1.f, 1.f, 1.f, // 7 // 17
         1.f, 1.f,-1.f, // 6 // 18
        -1.f, 1.f,-1.f, // 2 // 19

        // bottom       // v // ii
        -1.f,-1.f, 1.f, // 1 // 20
         1.f,-1.f, 1.f, // 5 // 21
         1.f,-1.f,-1.f, // 4 // 22
        -1.f,-1.f,-1.f, // 0 // 23
    };

    // Triangle indices: 6 faces * 2 triangles
    static const GLushort indices[36] =
    {
        // front
         0,  1,  2,
         2,  3,  0,

        // back
         4,  5,  6,
         6,  7,  4,

        // left
         8,  9, 10,
        10, 11,  8,

        // right
        12, 13, 14,
        14, 15, 12,

        // right
        16, 17, 18,
        18, 19, 16,

        // right
        20, 21, 22,
        22, 23, 20,

    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLushort), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Texture coordinates: 6 faces * 4 coordinates
    static const GLfloat texCoords[48] =
    {
        // front
        0.f, 0.f,
        0.5, 0.f,
        0.5, 0.5,
        0.f, 0.5,

        // back
        0.5, 0.5,
        1.f, 0.5,
        1.f, 1.f,
        0.5, 1.f,

        // left
        0.f, 0.f,
        0.5, 0.f,
        0.5, 0.5,
        0.f, 0.5,

        // right
        0.f, 0.f,
        0.5, 0.f,
        0.5, 0.5,
        0.f, 0.5,

        // top
        0.f, 0.5,
        0.5, 0.5,
        0.5, 1.f,
        0.f, 1.f,

        // bottom
        0.5, 0.f,
        1.f, 0.f,
        1.f, 0.5,
        0.5, 0.5,
    };

    glGenBuffers(1, &m_texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords) * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(a_texCoord);
    glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Initialize state
    glClearColor(0.2, 0.2, 0.2, 1.f);
    glUseProgram(m_program);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    return true;
}

Timer t;
float angleY(0.f);
float angleZ(0.f);
float angle (0.f);
GLfloat highlight(0.f);
long double clickevent(-1.f);

void Painter::paint()
{
    AbstractPainter::paint();

    if(t.paused())
        t.start();

    angleY +=  _deg(m_clockwise /  64.f);
    angleZ +=  _deg(m_clockwise / 128.f);
    angle  =  ((sin(_PI2 / 16.f) + 1.f) * 0.5 + 1.f) * 0.1337f;

    m_model = glm::rotate(glm::mat4(1), angleY, glm::vec3(0.f, 1.f, 0.f));
    m_model = glm::rotate(m_model, angleZ, glm::vec3(0.f, 0.f, 1.f));

    const glm::mat4 M = m_projection * m_view * m_model;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use program and set uniforms
    glUseProgram(m_program);

    // Bind texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    u_texture = glGetUniformLocation(m_program, "tex");
    glUniform1i(u_texture, 0);

    // Highlight object if clicked
    if (m_clicked)
    {
        clickevent = t.elapsed();
        highlight = 0.3f;
        m_clicked = !m_clicked;
    }
    else if (t.elapsed() < clickevent + 0.05f)
    {
        highlight = 0.3f;
    }
    else if (highlight > 0.f)
    {
        // Fade out
        highlight -= 0.02f;
        if (highlight < 0.f)
            highlight = 0.f;
    }

    glUniform1f(u_clicked, highlight);

    // Bind vertices
    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(a_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);

    glm::mat4 t = glm::translate(m_model, glm::vec3(0, 0, 0));
    t = glm::scale(t, glm::vec3(angle, angle, angle));

    const glm::mat4 T = m_projection * m_view * t;

    glUniformMatrix4fv(u_transform, 1, false, glm::value_ptr(T));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(a_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    glDisable(GL_TEXTURE_2D);


}

void Painter::resize(const int width, const int height)
{
    AbstractPainter::resize(width, height);

    m_width = width;
    m_height = height;

    m_aspect = static_cast<float>(width) / static_cast<float>(height);

    m_projection = glm::perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

bool Painter::objectDetected(const QPoint &position)
{
    GLfloat z;
    glReadPixels(position.x(), position.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

    if (z < 1.f)
    {
        m_clicked = true;
        return true;
    }
    m_clicked = false;
    return false;
}

void Painter::doMagic()
{
    m_clockwise *= -1;
}

const GLuint Painter::loadTexture(const QString & filePath)
{
    const QImage img = QImage(filePath);
    if (img.isNull())
    {
        qCritical("Loading image failed!");
        return -1;
    }

    QImage image = QGLWidget::convertToGLFormat(img);
    GLuint texture(-1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}
