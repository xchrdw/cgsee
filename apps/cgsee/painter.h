
#pragma once

#include <QString>
#include <QMap>

#include <core/abstractpainter.h>

class Camera;
class DataBlockRegistry;
class SceneGraph;
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
    DataBlockRegistry * m_registry;
    SceneGraph * m_scene;
    ScreenQuad * m_quad;

    Program * m_normalz;
    FrameBufferObject * m_fboNormalz;

    Program * m_flush;
    Camera * m_camera;
};
