#pragma once

#include <core/core_api.h>

#include <core/rendering/renderingpass.h>

class CORE_API ColorIdPass : public RenderingPass
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
