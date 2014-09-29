
#include <core/renderbufferobject.h>

#include <glbinding/glbinding.h>

RenderBufferObject::RenderBufferObject()
    : m_buffer(0)
    , m_internalFormat(gl::GLenum::GL_DEPTH32F_STENCIL8)
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
    gl::glFramebufferRenderbuffer(gl::GLenum::GL_FRAMEBUFFER, attachment, gl::GLenum::GL_RENDERBUFFER, m_buffer);

}

void RenderBufferObject::generateGLRenderBuffer()
{
    gl::glGenRenderbuffers(1, &m_buffer);

    gl::glBindRenderbuffer(gl::GLenum::GL_RENDERBUFFER, m_buffer);

    gl::glRenderbufferStorage(gl::GLenum::GL_RENDERBUFFER, m_internalFormat, m_width, m_height);

    gl::glBindRenderbuffer(gl::GLenum::GL_RENDERBUFFER, 0);

}

void RenderBufferObject::deleteGLRenderBuffer()
{
    if (m_buffer != 0)
    {
        gl::glDeleteRenderbuffers(1, &m_buffer);
    
    }
}

