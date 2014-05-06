#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

class Camera;
class Program;
class FrameBufferObject;

class CORE_API RenderTechnique
{
public:
    RenderTechnique(Camera & camera);
    virtual ~RenderTechnique();

    virtual void renderScene(const Program & program, FrameBufferObject * target = nullptr) = 0;

    virtual int preferredRefreshTimeMSec() const = 0;

    virtual void onInvalidatedView();
    virtual void onInvalidatedViewport(const int width, const int height);
    virtual void onInvalidatedChildren();

protected:
    Camera & m_camera;
};
