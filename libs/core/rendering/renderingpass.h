#pragma once

#include <glm/glm.hpp>
#include "../declspec.h"
#include "../camera.h"

class Group;
class Program;
class FrameBufferObject;
class FileAssociatedShader;

class CGSEE_API RenderingPass
{
public:
    RenderingPass(Camera * camera);
    virtual ~RenderingPass(void);

    bool isActive();
    void setActive(bool value);

    void applyIfActive();

    virtual void setUniforms() = 0;
    virtual void resize(const int width, const int height) = 0;

    virtual FrameBufferObject * output() = 0;
    virtual void clearFbos() = 0;
    virtual void sceneChanged(Group * scene);

protected:
    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);
    
    virtual void render() = 0;

    Camera * m_camera;

private:
    bool m_active;
};

