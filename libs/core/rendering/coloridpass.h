#pragma once

#include "renderingpass.h"

class CGSEE_API ColorIdPass : public RenderingPass
{
public:
    ColorIdPass(Camera * camera);
    virtual ~ColorIdPass(void);

    virtual void resize(const int width, const int height) override;

    virtual FrameBufferObject * output() override;
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;
    Program * m_program;
    FrameBufferObject * m_fbo;
};

