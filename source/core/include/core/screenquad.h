#pragma once

#include <core/core_api.h>

#include <globjects/globjects.h>

class BufferObject;
class Program;

class CORE_API ScreenQuad
{
public:
    ScreenQuad();
    virtual ~ScreenQuad();

    void draw(const Program & program) const;

protected:
    void initialize(const Program & program) const;

protected:  
    mutable gl::GLuint m_vao;
    mutable BufferObject * m_vertexBO;
};
