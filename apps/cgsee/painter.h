
#pragma once

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <QString>
#include <QMap>

#include <gui/abstractpainter.h>


class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual void paint();

    virtual void resize(
        const int width
    ,   const int height);

    const GLuint loadTexture(const QString & filePath);


protected:
    virtual const bool initialize();

protected:
	int m_width;
	int m_height;

	float m_aspect;

	float m_fovy;
	float m_zNear;
	float m_zFar;

	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;

	glm::mat4 m_projection;
	glm::mat4 m_model;
	glm::mat4 m_view;

    GLuint m_textureID;

	GLint m_vert;
	GLint m_frag;
	GLint m_program;

	GLint a_vertex;
	GLint u_transform;

	GLuint m_vao;
	GLuint m_indices;
    GLuint m_vertices;
};
