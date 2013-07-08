#pragma once

#include <glm/glm.hpp>

#include "Effect.h"
#include <vector>

class ScreenQuad;

class CGSEE_API BlurEffect : public Effect
{
public:
    BlurEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader, FrameBufferObject * fbo, FrameBufferObject * temp);
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
    FrameBufferObject * m_fboTemp;
    FrameBufferObject * m_fbo;
};

