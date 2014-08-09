#pragma once

#include <core/core_api.h>

#include <QVector>

#include <glbinding/gl/types.h>

#include <core/typedefs.h>
#include <core/gpuquery.h>


class CORE_API BufferObject
{
public:
    BufferObject(
        const gl::GLenum target
    ,   const gl::GLenum usage);

    virtual ~BufferObject();

    const gl::GLuint buffer();

    const gl::GLenum target() const;
    const gl::GLenum usage() const;

    template <typename T>
    void data(
        const QVector<T> & v
    ,   const gl::GLenum type
    ,   const gl::GLint  size);

    template <typename T>
    void data(
        const T * v
    ,   const gl::GLsizei count
    ,   const gl::GLenum type
    ,   const gl::GLint size);

    void bind();
    void bind(const gl::GLint attributeLocation);

    void release();
    void release(const gl::GLint attributeLocation);

    void draw(const gl::GLenum mode);

protected:
    inline const bool isBuffer() const;
    void generateBuffer();

protected:
    gl::GLuint m_buffer;

    const gl::GLenum  m_target;
    const gl::GLenum  m_usage;

    gl::GLsizei m_count;
    gl::GLint   m_size;
    gl::GLenum  m_type;
};


template <typename T>
void BufferObject::data(
    const QVector<T> & v
,   const gl::GLenum type
,   const gl::GLint  size)
{
    bind();

    m_count = static_cast<gl::GLsizei>(v.size());
    m_type  = type;
    m_size  = size;

    gl::glBufferData(m_target, m_count * sizeof(T), v.data(), m_usage);
    glError();
}

template <typename T>
void BufferObject::data(
    const T * v
,   const gl::GLsizei count
,   const gl::GLenum type
,   const gl::GLint size)
{
    bind();

    m_count = count;
    m_type  = type;
    m_size  = size;

    gl::glBufferData(m_target, m_count * sizeof(T), v, m_usage);
    glError();
}
