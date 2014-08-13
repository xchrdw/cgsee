#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>


class BufferObject;
class Program;

namespace glow{
    class FrameBufferObject;
}

class CORE_API ScreenQuad
{
public:
    ScreenQuad();
    virtual ~ScreenQuad();

    void draw(
        const Program & program
    ,   glow::FrameBufferObject * target = nullptr) const;

protected:
    void initialize(const Program & program) const;

protected:  
    mutable gl::GLuint m_vao;
    mutable BufferObject * m_vertexBO;
};
