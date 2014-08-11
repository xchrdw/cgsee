
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

,	m_height(1)
,	m_width (1)
,	m_aspect(1.f)

,	m_model(1.f)
,	m_view(1.f)
,	m_projection(1.f)

,	m_eye   (glm::vec3(0.f, 0.f,-4.f))
,	m_center(glm::vec3(0.f, 0.f, 0.f))
,	m_up    (glm::vec3(0.f, 1.f, 0.f))

,	m_fovy  (45.f)
,	m_zNear ( .1f)
,	m_zFar  (10.f)

,	m_vert(-1)
,	m_frag(-1)
,	m_program(-1)

,	a_vertex(-1)
,	u_transform(-1)

,	m_vao(-1)
,	m_indices(-1)
,   m_vertices(-1)
,   m_texCoords(-1)
,   m_textureID(0)
{
}

Painter::~Painter()
{
    glDeleteBuffers(1, &m_vertices);
    glDeleteBuffers(1, &m_texCoords);
    glDeleteBuffers(1, &m_indices);

	glDetachShader(m_program, m_vert);
	glError();
	glDetachShader(m_program, m_frag);
	glError();

	glDeleteShader(m_vert);
	glError();
	glDeleteShader(m_frag);
	glError();

	glDeleteProgram(m_program);
	glError();
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
        glError();

        GLchar *chr = new GLchar[maxLength];
        glGetShaderInfoLog(shader, maxLength, &logLength, chr);
        glError();

        log = chr;

		qCritical("Compiling shader failed.");
        qCritical("%s", log);
    }
}


const bool Painter::initialize()
{
    // initialize view

	m_view = glm::lookAt(m_eye, m_center, m_up);

	// initialize shaders and program
    const QString TEXTURE_FILENAME = "data/skycube.png";
    m_textureID = loadTexture(TEXTURE_FILENAME);

	glError();
	m_vert = glCreateShader(GL_VERTEX_SHADER);
	glError();
	m_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glError();

	static const char * srcv = // c++11

        "#version 150 core\n"
        "\n"
        "in vec3 a_vertex;\n"
        "in vec2 vertTexCoord;\n"
        "out vec2 fragTexCoord;\n"
		"\n"
		"\n"
		"uniform mat4 transform;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "   vec4 vertex = transform * vec4(a_vertex, 1.0);\n"
        "   fragTexCoord = vertTexCoord;\n"
        "	gl_Position = vertex;\n"
        "}\n";

	static const char * srcf =

        "#version 150 core\n"
        "\n"
        "uniform sampler2D tex;\n"
        "in vec2 fragTexCoord;\n"
        "out vec4 fragcolor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    float znear =  0.1;\n"
        "    float zfar  = 10.0;\n"
        "\n"
        "    //fragcolor = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "    fragcolor = texture(tex, fragTexCoord);\n"
        "}\n";

	glShaderSource(m_vert, 1, &srcv, nullptr);
	glError();
	glCompileShader(m_vert);
	glError();

	getCompileInfo(m_vert);

	glShaderSource(m_frag, 1, &srcf, nullptr);
	glError();
	glCompileShader(m_frag);
	glError();

	getCompileInfo(m_frag);


	m_program = glCreateProgram();

	glAttachShader(m_program, m_vert);
	glError();
	glAttachShader(m_program, m_frag);
	glError();

	glLinkProgram(m_program);
	glError();
	// get error log!

	a_vertex = glGetAttribLocation(m_program, "a_vertex");
	glError();

	u_transform = glGetUniformLocation(m_program, "transform");
	glError();

	// initialize geometry
	static const GLfloat vertices[24] =
    {
        -1.f,-1.f,-1.f,
	    -1.f,-1.f, 1.f,
		-1.f, 1.f,-1.f,
		-1.f, 1.f, 1.f,
		 1.f,-1.f,-1.f,
		 1.f,-1.f, 1.f,
		 1.f, 1.f,-1.f,
		 1.f, 1.f, 1.f
    };

    static const GLubyte indices[14] = {
        2, 0, 6, 4, 5, 0, 1, 2, 3, 6, 7, 5, 3, 1 };

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);
    glError();

    glGenBuffers(1, &m_indices);
	glError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (6 * 2 + 2) * sizeof(GLubyte), indices, GL_STATIC_DRAW);
	glError();

    glGenBuffers(1, &m_vertices);
	glError();
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glError();
	glBufferData(GL_ARRAY_BUFFER, (8 * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glError();


    // texture coordinates

    GLint a_vertTexCoord = glGetAttribLocation(m_program, "vertTexCoord");
    glError();

    GLfloat texCoords[] = {
        // front
        0.45, 0.55,
        0.5, 0.6,
        0.3, 0.4,
        0.2, 0.5,
        0.45, 0.55,
        0.5, 0.6,
        0.3, 0.4,
        0.2, 0.5
    };
    glGenBuffers(1, &m_texCoords);
    glError();
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
    glError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glError();
    glEnableVertexAttribArray(a_vertTexCoord);
    glError();
    glVertexAttribPointer(a_vertTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glError();

	// initialize state
    glClearColor(0.2, 0.2, 0.2, 1.f);
    glUseProgram(m_program);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	return true;
}

Timer t;
float angleY(0.f);
float angleZ(0.f);
float angle (0.f);

void Painter::paint()
{
    AbstractPainter::paint();
	glError();

    if(t.paused())
        t.start();

    angleY =  _deg(_PI2 * t.elapsed() / 8.f);
    angleZ =  _deg(sin(_PI * t.elapsed() / 16.f));
    angle  =  ((sin(_PI2 / 16.f) + 1.f) * 0.5 + 1.0) * 0.1f;

    m_model = glm::rotate(glm::mat4(1), angleY, glm::vec3(0.f, 1.f, 0.f));
    m_model = glm::rotate(m_model, angleZ, glm::vec3(0.f, 0.f, 1.f));

	const glm::mat4 M = m_projection * m_view * m_model;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use program and set uniforms

	glUseProgram(m_program);
	glError();

    // bind texture
    glEnable(GL_TEXTURE_2D);
    glError();
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glError();

    GLint u_texture = glGetUniformLocation(m_program, "tex");
    glError();

    glUniform1i(u_texture, 0);
    glError();


	// bind vertices
    glBindVertexArray(m_vao);
    glError();

    glEnableVertexAttribArray(a_vertex);
    glError();
    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glError();
    glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    glError();

    glm::mat4 t = glm::translate(m_model, glm::vec3(0, 0, 0));
    t = glm::scale(t, glm::vec3(angle, angle, angle));

    const glm::mat4 T = m_projection * m_view * t;

    glUniformMatrix4fv(u_transform, 1, false, glm::value_ptr(T));
    glError();

    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, 0);
    glError();

    glDisableVertexAttribArray(a_vertex);
    glError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glError();

	glUseProgram(0);
	glError();

    glDisable(GL_TEXTURE_2D);
    glError();

}

void Painter::resize(
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);

	m_width = width;
	m_height = height;

	m_aspect = static_cast<float>(width) / static_cast<float>(height);

	m_projection = glm::perspective(m_fovy, m_aspect, m_zNear, m_zFar);
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
