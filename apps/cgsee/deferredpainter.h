
#pragma once

#include <QString>
#include <QMap>

#include <core/abstractpainter.h>


class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class DeferredPainter : public AbstractPainter
{
public:
    DeferredPainter();
    virtual ~DeferredPainter();

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
    Camera * m_camera;
    Group * m_group;
    ScreenQuad * m_quad;

    Program * m_normalz;
    FrameBufferObject * m_fboNormalz;

    Program * m_edgeEnhancement;
    FrameBufferObject * m_fboEdges;

    Program * m_unsharpMaskingDepthBuffer;
    FrameBufferObject * m_fboUnsharpMaskingDepthBuffer;

    Program * m_flush;
};
