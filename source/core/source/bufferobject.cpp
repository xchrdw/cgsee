
#include <core/bufferobject.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <core/gpuquery.h>


BufferObject::BufferObject(
    const gl::GLenum target
,   const gl::GLenum usage)

:   m_buffer(-1)
,   m_target(target)
,   m_usage(usage)

,   m_count(0)
,   m_size(0)

,   m_type(gl::GL_NONE)
{
}

BufferObject::~BufferObject()
{
    if(isBuffer())
    {
        gl::glDeleteBuffers(1, &m_buffer);
        glError();
    }
}

inline const bool BufferObject::isBuffer() const
{
    return m_buffer != -1;
}

const gl::GLuint BufferObject::buffer()
{
    if(!isBuffer())
        generateBuffer();

    return m_buffer;
}

const gl::GLenum BufferObject::target() const
{
    return m_target;
}

const gl::GLenum BufferObject::usage() const
{
    return m_usage;
}

void BufferObject::bind()
{
    if(!isBuffer())
        generateBuffer();

    glBindBuffer(m_target, m_buffer);
    glError();
}

void BufferObject::bind(const gl::GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    assert(m_target == gl::GL_ARRAY_BUFFER);

    bind();

    gl::glEnableVertexAttribArray(attributeLocation);
    glError();
    gl::glVertexAttribPointer(attributeLocation, m_size, m_type, gl::GL_FALSE, 0, 0);
    glError();
}

void BufferObject::draw(const gl::GLenum mode)
{
    switch(m_target)
    {
    case gl::GL_ELEMENT_ARRAY_BUFFER:
        glDrawElements(mode, m_count, m_type, 0);
        break;
    case gl::GL_ARRAY_BUFFER:
        gl::glDrawArrays(mode, 0, m_count);
        break;
    };
    glError();
}

void BufferObject::release(const gl::GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    if(!isBuffer())
        return;

    gl::glDisableVertexAttribArray(attributeLocation);
    glError();

    gl::glBindBuffer(m_target, 0);
    glError();
}

void BufferObject::release()
{
    if(!isBuffer())
        return;

    gl::glBindBuffer(m_target, 0);
    glError();
}

void BufferObject::generateBuffer()
{
    if(isBuffer())
        return;

    gl::glGenBuffers(1, &m_buffer);
    glError();
}