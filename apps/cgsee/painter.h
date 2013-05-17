#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <QString>
#include <QMap>

#include <core/abstractpainter.h>



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
    virtual Camera * camera();

protected:
    void postShaderRelinked();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    static void bindSampler(
        const t_samplerByName & sampler
    ,   const Program & program);

    static void releaseSampler(
        const t_samplerByName & sampler);

protected:
    Group * m_group;
    ScreenQuad * m_quad;

	Program * m_normalz;
	Program * m_flat;
	Program * m_gouraud;
	Program * m_phong;
	Program * m_gooch;
	Program * m_useProgram;
    FrameBufferObject * m_fboNormalz;
	
	glm::vec3 camPos;
	
    Program * m_flush;
    Camera * m_camera;
};
