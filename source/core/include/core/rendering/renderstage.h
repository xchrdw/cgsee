#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <core/property/valueproperty.h>
#include <core/painter/pipelinepainter.h>

class Group;
class Program;
class FrameBufferObject;

class CORE_API RenderStage
{
public:
    typedef GLuint FrameBuffer;

    RenderStage(PipelinePainter & painter);
    virtual ~RenderStage(void);

    virtual void reloadShaders() = 0;
    virtual void resize(const int width, const int height) = 0;
    virtual void render() = 0;

protected:
    void drawScene(glm::mat4 & transform, Program * program);

    bool isSceneInvalid();
    bool isViewInvalid();


protected:
    PipelinePainter & m_painter;
};
