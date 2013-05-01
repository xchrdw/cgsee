
#pragma once

#include <GL/glew.h>

#include "declspec.h"


class Program;
class FrameBufferObject;

class CGSEE_API ScreenQuad
{
public:
    ScreenQuad();
    virtual ~ScreenQuad();

    void draw(
        FrameBufferObject * target = nullptr) const;
    void draw(
        const Program & program
    ,   FrameBufferObject * target = nullptr) const;
    void draw(
        const GLuint program
    ,   const GLint vertexAttributeLocation
    ,   FrameBufferObject * target = nullptr) const;

protected:
    void initialize() const;
    void initializeDefaultProgram() const;

    inline const bool isBuffer() const;

protected:  
    mutable GLuint m_buffer;
    mutable GLuint m_vao;
    
    mutable Program * m_program;
    mutable GLint m_vertex;
};