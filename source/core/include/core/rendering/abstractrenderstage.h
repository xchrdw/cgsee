#pragma once

#include <core/core_api.h>

#include <globjects/globjects.h>

#include <core/property/valueproperty.h>
#include <core/painter/pipelinepainter.h>

class Group;
class Program;

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

    gl::GLuint m_fbo;
    bool m_fboIsBound;

};
