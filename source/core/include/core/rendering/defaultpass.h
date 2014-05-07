#pragma once

#include <core/core_api.h>

#include <core/rendering/renderingpass.h>

// provides a program, a fbo and default implementations for resize(), output(), clear() and render()
class CORE_API DefaultPass : public RenderingPass
{
public:
    DefaultPass(Camera * camera);
    virtual ~DefaultPass(void);

    virtual void resize(const int width, const int height) override;

    virtual FrameBufferObject * output() override;
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;
    Program * m_program;
    FrameBufferObject * m_fbo;
};
