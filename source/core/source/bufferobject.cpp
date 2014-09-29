
#include <core/bufferobject.h>

#include <glbinding/glbinding.h>

BufferObject::BufferObject(
    const gl::GLenum target
,   const gl::GLenum usage)

:   m_buffer(-1)
,   m_target(target)
,   m_usage(usage)

,   m_count(0)
,   m_size(0)

,   m_type(gl::GLenum::GL_NONE)
{
}

BufferObject::~BufferObject()
{
    if(isBuffer())
    {
        gl::glDeleteBuffers(1, &m_buffer);
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

    gl::glBindBuffer(m_target, m_buffer);

}

void BufferObject::bind(const gl::GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    assert(m_target == gl::GLenum::GL_ARRAY_BUFFER);

    bind();

    gl::glEnableVertexAttribArray(attributeLocation);

    gl::glVertexAttribPointer(attributeLocation, m_size, m_type, gl::GL_FALSE, 0, 0);

}

void BufferObject::draw(const gl::GLenum mode)
{
    switch(m_target)
    {
    case gl::GLenum::GL_ELEMENT_ARRAY_BUFFER:
        gl::glDrawElements(mode, m_count, m_type, 0);
        break;
    case gl::GLenum::GL_ARRAY_BUFFER:
        gl::glDrawArrays(mode, 0, m_count);
        break;
    };
}

void BufferObject::release(const gl::GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    if(!isBuffer())
        return;

    gl::glDisableVertexAttribArray(attributeLocation);


    gl::glBindBuffer(m_target, 0);

}

void BufferObject::release()
{
    if(!isBuffer())
        return;

    gl::glBindBuffer(m_target, 0);

}

void BufferObject::generateBuffer()
{
    if(isBuffer())
        return;

    gl::glGenBuffers(1, &m_buffer);

}