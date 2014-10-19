#pragma once
#include <core/core_api.h>

#include <glbinding/gl/types.h>

#include <glm/mat4x4.hpp>

#include <core/painter/pipelinepainter.h>

class Group;

namespace globjects{
    class Framebuffer;
    class Program;
}

class CORE_API AbstractRenderStage
{
public:
    AbstractRenderStage(PipelinePainter & painter);
    virtual ~AbstractRenderStage();

    virtual void reloadShaders() = 0;
    virtual void resize(const int width, const int height) = 0;
    virtual void render() = 0;

    PipelinePainter & painter();

protected:
    void drawScene(const glm::mat4 & transform, globjects::Program * program);

    bool isSceneInvalid();
    bool isViewInvalid();

protected:
    void bindFBO();
    void releaseFBO();

protected:
    PipelinePainter & m_painter;

    globjects::Framebuffer *m_fbo;
    bool m_fboIsBound;
};
