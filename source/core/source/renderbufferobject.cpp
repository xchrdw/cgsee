
#include <core/renderbufferobject.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

RenderBufferObject::RenderBufferObject()
    : m_buffer(0)
    , m_internalFormat(gl::GL_DEPTH32F_STENCIL8)
{

}

RenderBufferObject::RenderBufferObject(gl::GLenum internalFormat)
    : m_buffer(0)
    , m_internalFormat(internalFormat)
{

}

RenderBufferObject::RenderBufferObject(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat)
    : m_buffer(0)
    , m_width(width)
    , m_height(height)
    , m_internalFormat(internalFormat)
{
    generateGLRenderBuffer();
}

RenderBufferObject::~RenderBufferObject()
{
    deleteGLRenderBuffer();
}

bool RenderBufferObject::isInitialized()
{
    return m_buffer != 0;
}

void RenderBufferObject::configure(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat)
{
    m_width = width;
    m_height = height;
    m_internalFormat = internalFormat;

    deleteGLRenderBuffer();
    generateGLRenderBuffer();
}

void RenderBufferObject::resize(gl::GLuint width, gl::GLuint height)
{
    m_width = width;
    m_height = height;

    deleteGLRenderBuffer();
    generateGLRenderBuffer();
}

void RenderBufferObject::attachTo(gl::GLenum attachment)
{
    gl::glFramebufferRenderbuffer(gl::GL_FRAMEBUFFER, attachment, gl::GL_RENDERBUFFER, m_buffer);
    glError();
}

void RenderBufferObject::generateGLRenderBuffer()
{
    gl::glGenRenderbuffers(1, &m_buffer);
    glError();
    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, m_buffer);
    glError();
    gl::glRenderbufferStorage(gl::GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
    glError();
    gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, 0);
    glError();
}

void RenderBufferObject::deleteGLRenderBuffer()
{
    if (m_buffer != 0)
    {
        gl::glDeleteRenderbuffers(1, &m_buffer);
        glError();
    }
}

