
#include <core/renderbufferobject.h>

#include <GL/glew.h>

RenderBufferObject::RenderBufferObject()
    : m_buffer(0)
    , m_internalFormat(GL_DEPTH32F_STENCIL8)
{

}

RenderBufferObject::RenderBufferObject(GLenum internalFormat)
    : m_buffer(0)
    , m_internalFormat(internalFormat)
{

}

RenderBufferObject::RenderBufferObject(GLuint width, GLuint height, GLenum internalFormat)
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

void RenderBufferObject::configure(GLuint width, GLuint height, GLenum internalFormat)
{
    m_width = width;
    m_height = height;
    m_internalFormat = internalFormat;

    deleteGLRenderBuffer();
    generateGLRenderBuffer();
}

void RenderBufferObject::resize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;

    deleteGLRenderBuffer();
    generateGLRenderBuffer();
}

void RenderBufferObject::attachTo(GLenum attachment)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_buffer);
    glError();
}

void RenderBufferObject::generateGLRenderBuffer()
{
    glGenRenderbuffers(1, &m_buffer);
    glError();
    glBindRenderbuffer(GL_RENDERBUFFER, m_buffer);
    glError();
    glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
    glError();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glError();
}

void RenderBufferObject::deleteGLRenderBuffer()
{
    if (m_buffer != 0)
    {
        glDeleteRenderbuffers(1, &m_buffer);
        glError();
    }
}

