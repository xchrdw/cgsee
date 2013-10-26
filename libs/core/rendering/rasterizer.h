#pragma once

#include <glm/glm.hpp>

#include <core/declspec.h>
#include "rendertechnique.h"


class Program;

class CGSEE_API Rasterizer : public RenderTechnique
{
public:
    Rasterizer(Camera & abstraction);
    virtual ~Rasterizer();
    
    virtual void renderScene(const Program & program, FrameBufferObject * target = nullptr) override;

    virtual int preferredRefreshTimeMSec() const { return -1; };
};
