
#pragma once

#include <QString>
#include <QMap>

#include <gui/abstractpainter.h>

#include <glm/gtc/matrix_transform.hpp>


class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual void paint();

    virtual void resize(
        const int width
    ,   const int height);

protected:
    virtual const bool initialize();

protected:
    void postShaderRelinked();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

	static void bindSampler(
		const t_samplerByName & sampler
	,	Program * program);

	static void releaseSampler(
		const t_samplerByName & sampler);

protected:
    Camera * m_camera;
    Group * m_group;
    ScreenQuad * m_quad;

	glm::vec3 camPos;

    Program * m_normalz;
	Program * m_flat;
	Program * m_gouraud;
	Program * m_phong;
	Program * m_gooch;
	Program * m_useProgram;
    FrameBufferObject * m_fboNormalz;
	int count;

    Program * m_flush;
};
