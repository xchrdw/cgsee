#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>


class BufferObject;

namespace globjects{
    class Framebuffer;
    class Program;
}

class CORE_API ScreenQuad
{
public:
    ScreenQuad();
    virtual ~ScreenQuad();

    void draw(
        globjects::Program & program
    ,   globjects::Framebuffer * target = nullptr) const;

protected:
    void initialize(globjects::Program & program) const;

protected:  
    mutable gl::GLuint m_vao;
    mutable BufferObject * m_vertexBO;
};
