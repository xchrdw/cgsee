#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <QString>
#include <QMap>

#include "abstractscenepainter.h"



class DataBlockRegistry;
class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class IdPainter : public AbstractScenePainter
{
public:
    IdPainter(Camera * camera);
    IdPainter(Group * scene);
    virtual ~IdPainter();

    virtual void paint();

    virtual void setShading(char shader);

    virtual void resize(
        const int width
    ,   const int height);
protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

protected:
    void postShaderRelinked();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    // static void bindSampler(
    //     const t_samplerByName & sampler
    // ,   const Program & program);

    // static void releaseSampler(
    //     const t_samplerByName & sampler);
   
protected:
    ScreenQuad * m_quad;

    Program * m_program;
    FrameBufferObject * m_fbo;


    // glm::vec3 camPos;

    Program * m_flush;

    Camera * m_camera;
};
