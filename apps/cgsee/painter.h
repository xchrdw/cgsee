
#pragma once

#include <QString>
#include <QMap>

#include <core/abstractscenepainter.h>


class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class Painter : public AbstractScenePainter
{
public:
    Painter();
    Painter(Group * scene);
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
    ScreenQuad * m_quad;

    Program * m_normalz;
    FrameBufferObject * m_fboNormalz;

    Program * m_flush;
    Camera * m_camera;
};
