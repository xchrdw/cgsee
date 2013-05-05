
#pragma once

#include <QString>
#include <QMap>

#include <gui/abstractpainter.h>


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
    ,   const Program & program);

    static void releaseSampler(
        const t_samplerByName & sampler);

protected:
    Camera * m_camera;
    Group * m_group;
    ScreenQuad * m_quad;

    Program * m_normalz;
    FrameBufferObject * m_fboNormalz;

    Program * m_flush;
};
