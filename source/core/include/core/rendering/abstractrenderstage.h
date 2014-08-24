#pragma once

#include <glbinding/gl/types.h>

#include <glm/mat4x4.hpp>

#include <core/core_api.h>

class Group;
class Program;
class PipelinePainter;

namespace glo{
    class FrameBufferObject;
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
    void drawScene(const glm::mat4 & transform, Program * program);

    bool isSceneInvalid();
    bool isViewInvalid();

protected:
    void bindFBO();
    void releaseFBO();

    
    
    gl::GLuint createRenderbuffer(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat);
    void deleteRenderbuffer(gl::GLuint renderbuffer);
    void attachRenderbuffer(gl::GLenum attachment, gl::GLuint renderbuffer);

protected:
    PipelinePainter & m_painter;

    glo::FrameBufferObject *m_fbo;
    bool m_fboIsBound;
};
