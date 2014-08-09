#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <glbinding/gl/types.h>

#include <core/typedefs.h>


class Program;

class CORE_API FrameBufferObject
{
public:
    FrameBufferObject(
        const gl::GLenum internalFormat
    ,   const gl::GLenum format
    ,   const gl::GLenum type
    ,   const gl::GLenum attachment
    ,   const bool   depth);

    virtual ~FrameBufferObject();

    void bind() const;
    void release() const;

    void bindTexture2D(
        const Program & program
    ,   const QString & uniform
    ,   const glm::uint slot) const;

    void releaseTexture2D() const;

    void clear();

    void resize(
        const unsigned int width
    ,   const unsigned int height);

    unsigned int width();
    unsigned int height();

protected:
    void initialize() const;
    void resize() const;

    inline const bool isFrameBuffer() const;
    inline const bool isRenderBuffer() const;
    inline const bool isTexture() const;
public:
    mutable gl::GLuint m_fbo;
protected:
    mutable gl::GLuint m_render;
    mutable gl::GLuint m_texture;

    glm::ivec2 m_size;

    const gl::GLenum m_internal;
    const gl::GLenum m_format;
    const gl::GLenum m_type;
    const gl::GLenum m_attachment;
    const bool   m_depth;

    unsigned int m_width;
    unsigned int m_height;
};
