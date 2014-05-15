#pragma once

#include <core/core_api.h>

#include <core/property/valueproperty.h>

class Group;
class Program;
class FrameBufferObject;
class FrameBufferProvider;

class CORE_API RenderStage
{
public:
    RenderStage(Painter & painter);
    virtual ~RenderStage(void);

    virtual void reloadShaders() = 0;
    virtual void resize(const int width, const int height) = 0;
    virtual void render() = 0;

protected:
    void drawScene(glm::mat4 & transform, Program * program, FrameBufferObject * fbo);

    FrameBufferObject *getFramebuffer(u_int32_t slot);
    void setFramebuffer(u_int32_t slot, FrameBufferObject fbo);
    void unsetFramebuffer(u_int32_t slot);

    bool isSceneInvalid();
    bool isViewInvalid();


protected:
    Painter & m_painter;
};
