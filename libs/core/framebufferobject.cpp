
#include "framebufferobject.h"

#include "gpuquery.h"
#include "program.h"


// http://www.swiftless.com/tutorials/opengl/framebuffer.html

FrameBufferObject::FrameBufferObject(
    const GLenum internalFormat
,   const GLenum format
,   const GLenum type
,   const GLenum attachment
,   const bool   depth
,   const bool   multisampling
,   const unsigned int samples)

:   m_fbo(-1)
,   m_render(-1)

,   m_size(glm::ivec2(0))

,   m_internal(internalFormat)
,   m_format(format)
,   m_type(type)
,   m_attachment(attachment)
,   m_depth(depth)
,   m_multisampling(multisampling)
,   m_samples(samples)
{
}

FrameBufferObject::~FrameBufferObject()
{
    for(t_textures::const_iterator texture = m_textures.constBegin(); texture != m_textures.constEnd(); ++texture)
    {
        if(isTexture(texture.value()))
        {
            // TODO
            glDeleteTextures(1, &texture.value());
            glError();
        }
    }
    if(isRenderBuffer())
    {
        glDeleteRenderbuffers(1, &m_render);
        glError();
    }
    if(isFrameBuffer())
    {
        glDeleteFramebuffers(1, &m_fbo);
        glError();
    }
}

inline const bool FrameBufferObject::isTexture(const GLuint texture) const
{
    return texture != 0;
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

    glViewport(0, 0, m_size.x, m_size.y);
    glError();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();

    GLenum buffers[3] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT};
    glDrawBuffers(3, buffers);
    glError();
}

void FrameBufferObject::release() const
{
    if(!isFrameBuffer())
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glError();

    GLenum buffers2[1] = {GL_FRONT_LEFT};
    glDrawBuffers(1, buffers2);
    glError();
}

void FrameBufferObject::bindTexture2D(
    const Program & program
,   const QString & uniform
,   const glm::uint slot) const
{
    if(!m_textures.contains(slot))
        m_textures[slot] = 0;

    if(!isTexture(m_textures[slot]))
        initialize();

    glActiveTexture(GL_TEXTURE0 + slot);
    glError();

    glBindTexture(m_multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_textures[slot]);
    glError();

    program.setUniform(uniform, slot);
}

void FrameBufferObject::releaseTexture2D(const glm::uint slot) const
{
    if(!m_textures.contains(slot) || !isTexture(m_textures[slot]))
        return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glError();

    glBindTexture(m_multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);
    glError();
}

void FrameBufferObject::initialize() const
{
    if(m_depth) // Initialize Render Buffer for Depth
    {
        glGenRenderbuffers(1, &m_render);
        glError();
    }

    for(t_textures::const_iterator texture = m_textures.constBegin(); texture != m_textures.constEnd(); ++texture)
    {
        if(!isTexture(texture.value()))
        {
            GLuint textureID;
            glGenTextures(1, &textureID);
            m_textures[texture.key()] = textureID;
            glError();
        }
    }

    glGenFramebuffers(1, &m_fbo);
    glError();

    resize();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();

    if(m_depth)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_render);
        glError();

        if(!m_multisampling)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render);
        else {
            glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_samples, GL_DEPTH_COMPONENT32, m_size.x, m_size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render);
        }

        glError();
    }

    for(t_textures::const_iterator texture = m_textures.constBegin(); texture != m_textures.constEnd(); ++texture)
    {
        if(isTexture(texture.value()))
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachment + texture.key(), m_multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture.value(), 0);
            glError();
        }
    }

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glError();

    if(GL_FRAMEBUFFER_COMPLETE != status)
        qCritical("Frame Buffer Object incomplete.");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glError();
}

void FrameBufferObject::resize(
    const unsigned int width
,   const unsigned int height)
{
    m_size = glm::ivec2(width, height);

    resize();
}

void FrameBufferObject::resize() const
{
    if(m_depth && m_render != -1)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_render);
        glError();

        if(!m_multisampling)
             glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_size.x, m_size.y);
        else
             glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, m_samples, GL_DEPTH_COMPONENT32, m_size.x, m_size.y);
            //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_size.x, m_size.y);

        glError();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glError();
    }

    for(t_textures::const_iterator texture = m_textures.constBegin(); texture != m_textures.constEnd(); ++texture)
    {
        if(isTexture(texture.value()))
        {
            glActiveTexture(GL_TEXTURE0 + texture.key());
            glError();

            glBindTexture(m_multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture.value());
            glError();

            if(!m_multisampling)
                glTexImage2D(GL_TEXTURE_2D, 0, m_internal, m_size.x, m_size.y, 0, m_format, m_type, 0);
            else
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, m_internal, m_size.x, m_size.y, false);

            glError();

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glBindTexture(m_multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 0);
            glError();
        }
    }
}
