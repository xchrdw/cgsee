#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <core/property/valueproperty.h>
#include <core/painter/pipelinepainter.h>

class Group;
class Program;

class CORE_API AbstractRenderStage
{
public:
    typedef GLuint FrameBuffer;

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

    GLuint createRenderbuffer(GLuint width, GLuint height, GLenum internalFormat);
    void deleteRenderbuffer(GLuint renderbuffer);
    void attachRenderbuffer(GLenum attachment, GLuint renderbuffer);

protected:
    PipelinePainter & m_painter;

    GLuint m_fbo;
    bool m_fboIsBound;

};
