#pragma once
#include <glm/glm.hpp>
//
//#include <QVector>
//
#include <core/declspec.h>
#include <core/camera.h>
//#include "scenegraph/group.h"

class Program;

class CGSEE_API RenderTechnique
{
public:
    RenderTechnique(Camera & camera);
    virtual ~RenderTechnique();

    virtual void renderScene(const Program & program, const glm::mat4 & transform) = 0;

    virtual int preferredRefreshTimeMSec() const = 0;

    virtual void onInvalidatedView();
    virtual void onInvalidatedViewport(const int width, const int height);
    virtual void onInvalidatedChildren();

protected:
    Camera & m_camera;
};
