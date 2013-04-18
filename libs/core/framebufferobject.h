#pragma once

#include <GL/glew.h>

#include "common.h"

#include <glm/glm.hpp>

class Program;

class FrameBufferObject
{
public:
    FrameBufferObject(
        const GLenum internalFormat
    ,   const GLenum format
    ,   const GLenum type
    ,   const GLenum attachment
    ,   const bool   depth);

    virtual ~FrameBufferObject();

    void bind() const;
    void release() const;

    void bindTexture2D(
        Program * program
    ,   const QString & uniform
    ,   const glm::uint slot) const;

    void releaseTexture2D() const;

    void FrameBufferObject::resize(
        const unsigned int width
    ,   const unsigned int height);

protected:
    void initialize() const;
    void resize() const;

protected:
    mutable GLuint m_fbo;
    mutable GLuint m_render;
    mutable GLuint m_texture;

    glm::ivec2 m_size;

    const GLenum m_internal;
    const GLenum m_format;
    const GLenum m_type;
    const GLenum m_attachment;
    const bool   m_depth;
};