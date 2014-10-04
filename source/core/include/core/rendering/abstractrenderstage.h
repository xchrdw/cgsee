#pragma once

#include <glbinding/gl/types.h>

#include <glm/mat4x4.hpp>

#include <core/core_api.h>

class Group;
class Program;
class PipelinePainter;

namespace globjects{
    class Framebuffer;
    class Program;
}

class CORE_API AbstractRenderStage
{
public:
    AbstractRenderStage(PipelinePainter & painter);
    virtual ~AbstractRenderStage(void);

    virtual void reloadShaders() = 0;
    virtual void resize(const int width, const int height) = 0;
    virtual void render() = 0;

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
