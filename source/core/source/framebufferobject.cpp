
#include <core/framebufferobject.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <core/gpuquery.h>
#include <core/program.h>


// http://www.swiftless.com/tutorials/opengl/framebuffer.html

FrameBufferObject::FrameBufferObject(
    const gl::GLenum internalFormat
,   const gl::GLenum format
,   const gl::GLenum type
,   const gl::GLenum attachment
,   const bool   depth)

:   m_fbo(-1)
,   m_render(-1)
,   m_texture(-1)

,   m_size(glm::ivec2(0))

,   m_internal(internalFormat)
,   m_format(format)
,   m_type(type)
,   m_attachment(attachment)
,   m_depth(depth)
,   m_width(0)
,   m_height(0)
{
}

FrameBufferObject::~FrameBufferObject()
{
    if(isTexture())
    {
        gl::glDeleteTextures(1, &m_texture);
        glError();
    }
    if(isRenderBuffer())
    {
        gl::glDeleteRenderbuffers(1, &m_render);
        glError();
    }
    if(isFrameBuffer())
    {
        gl::glDeleteFramebuffers(1, &m_fbo);
        glError();
    }
}

inline const bool FrameBufferObject::isTexture() const
{
    return m_texture != -1;
}

inline const bool FrameBufferObject::isRenderBuffer() const
{
    return m_render != -1;
}

inline const bool FrameBufferObject::isFrameBuffer() const
{
    return m_fbo != -1;
}

void FrameBufferObject::bind() const
{
    if(!isFrameBuffer())
        initialize();

    gl::glViewport(0, 0, m_size.x, m_size.y);
    glError();

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, m_fbo);
    glError();
}

void FrameBufferObject::release() const
{
    if(!isFrameBuffer())
        return;

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
    glError();
}

void FrameBufferObject::bindTexture2D(
    const Program & program
,   const QString & uniform
,   const glm::uint slot) const
{
    if(!isTexture())
        initialize();

    gl::glActiveTexture(gl::GL_TEXTURE0 + slot);
    glError();

    gl::glBindTexture(gl::GL_TEXTURE_2D, m_texture);
    glError();

    program.setUniform(uniform, gl::GLint(slot));
}

void FrameBufferObject::releaseTexture2D() const
{
    if(!isTexture())
        return;

    gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
    glError();
}

void FrameBufferObject::initialize() const
{
    if(m_depth) // Initialize Render Buffer for Depth
    {
        gl::glGenRenderbuffers(1, &m_render);
        glError();
    }

    gl::glGenTextures(1, &m_texture);
    glError();

    gl::glGenFramebuffers(1, &m_fbo);
    glError();

    resize();

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, m_fbo);
    glError();

    if(m_depth)
    {
        gl::glFramebufferRenderbuffer(gl::GL_FRAMEBUFFER, gl::GL_DEPTH_ATTACHMENT, gl::GL_RENDERBUFFER, m_render);
        glError();
    }

    glFramebufferTexture2D(gl::GL_FRAMEBUFFER, m_attachment
        , gl::GL_TEXTURE_2D, m_texture, 0);
    glError();

    const gl::GLenum status = gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER);
    glError();

    if(gl::GL_FRAMEBUFFER_COMPLETE != status)
        qCritical("Frame Buffer Object incomplete.");

    gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
    glError();
}

void FrameBufferObject::resize(
    const unsigned int width
,   const unsigned int height)
{
    m_size = glm::ivec2(width, height);

    m_width = width;
    m_height = height;

    resize();
}

unsigned int FrameBufferObject::width()
{
    return m_width;
}

unsigned int FrameBufferObject::height()
{
    return m_height;
}

void FrameBufferObject::resize() const
{
    if(m_depth && m_render != -1)
    {
        gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, m_render);
        glError();
        gl::glRenderbufferStorage(gl::GL_RENDERBUFFER, gl::GL_DEPTH_COMPONENT24, m_size.x, m_size.y);
        glError();
        gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, 0);
        glError();
    }

    if(m_texture != -1)
    {
        gl::glBindTexture(gl::GL_TEXTURE_2D, m_texture);
        glError();

        gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, static_cast<gl::GLint>(m_internal), m_size.x, m_size.y, 0, m_format, m_type, 0);
        glError();

        gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
        gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
        gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
        gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));

        gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
        glError();
    }
}

void FrameBufferObject::clear()
{
    bind();
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    release();
}
