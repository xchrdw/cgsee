#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>


class BufferObject;

namespace glo{
    class FrameBufferObject;
    class Program;
}

class CORE_API ScreenQuad
{
public:
    ScreenQuad();
    virtual ~ScreenQuad();

    void draw(
        glo::Program & program
    ,   glo::FrameBufferObject * target = nullptr) const;

protected:
    void initialize(glo::Program & program) const;

protected:  
    mutable gl::GLuint m_vao;
    mutable BufferObject * m_vertexBO;
};
