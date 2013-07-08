#pragma once

#include <glm/glm.hpp>

#include "renderingpass.h"
#include <vector>

class ScreenQuad;

class CGSEE_API BlurEffect : public RenderingPass
{
public:
    BlurEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader, RenderingPass * target, FrameBufferObject * temp);
    virtual ~BlurEffect(void);

    virtual void resize(const int width, const int height) override;
    virtual FrameBufferObject * output() override;
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;

private:
    Program * m_blurv;
    Program * m_blurh;
    ScreenQuad * m_quad;
    RenderingPass * m_target;
    FrameBufferObject * m_fboTemp;
};

