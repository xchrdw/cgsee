#pragma once
#include <glm/glm.hpp>

#include <core/declspec.h>
#include <core/camera.h>

class Program;
class FrameBufferObject;

class CGSEE_API RenderTechnique
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
