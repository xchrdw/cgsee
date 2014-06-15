
#include <core/framebufferobject.h>

#include <core/gpuquery.h>
#include <core/program.h>


// http://www.swiftless.com/tutorials/opengl/framebuffer.html

FrameBufferObject::FrameBufferObject(
    const GLenum internalFormat
,   const GLenum format
,   const GLenum type
,   const GLenum attachment
,   const bool   depth)

:   m_fbo(-1)
,   m_render(-1)
,   m_texture(-1)

,   m_size(glm::ivec2(0))

,   m_internal(internalFormat)
,   m_format(format)
,   m_type(type)
,	m_layerCount(0)
,   m_attachment(attachment)
,   m_depth(depth)
,   m_width(0)
,	m_height(0)
{
}

FrameBufferObject::FrameBufferObject(
    const GLenum internalFormat
    , const GLenum  format
    , const GLenum  type
    , const GLenum  startAttachment
    , const GLsizei layerCount
    , const bool    depth)

    : m_fbo(-1)
    , m_render(-1)
    , m_texture(-1)

    , m_size(glm::ivec2(0))

    , m_internal(internalFormat)
    , m_format(format)
    , m_type(type)
    , m_layerCount(layerCount)
    , m_attachment(startAttachment)
    , m_multiAttachment(new GLenum[layerCount])
    , m_depth(depth)
    , m_width(0)
    , m_height(0)
{
    for (int i = 0; i < layerCount; i++)
    {
        m_multiAttachment[i] = m_attachment + i;
    }
}

FrameBufferObject::~FrameBufferObject()
{
    if(isTexture())
    {
        glDeleteTextures(1, &m_texture);
        glError();
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

    glViewport(0, 0, m_size.x, m_size.y);
    glError();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();
}

void FrameBufferObject::release() const
{
    if(!isFrameBuffer())
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glError();
}

void FrameBufferObject::bindTexture2D(
    const Program & program
,   const QString & uniform
,   const glm::uint slot) const
{
    if(!isTexture())
        initialize();

    glActiveTexture(GL_TEXTURE0 + slot);
    glError();

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glError();

    program.setUniform(uniform, GLint(slot));
}

void FrameBufferObject::bindTexture3D(
	const Program & program
	, const QString & uniform
	, const glm::uint slot) const
{
	if (!isTexture())
		initialize();

	glActiveTexture(GL_TEXTURE0 + slot);
	glError();

	glBindTexture(GL_TEXTURE_3D, m_texture);
	glError();

	program.setUniform(uniform, GLint(slot));
}

void FrameBufferObject::releaseTexture2D() const
{
    if(!isTexture())
        return;

    glBindTexture(GL_TEXTURE_2D, 0);
    glError();
}

void FrameBufferObject::releaseTexture3D() const
{
	if (!isTexture())
		return;

	glBindTexture(GL_TEXTURE_3D, 0);
	glError();
}

void FrameBufferObject::initialize() const
{
    if(m_depth) // Initialize Render Buffer for Depth
    {
        if (m_layerCount <= 0)
        {
            glGenRenderbuffers(1, &m_render);
            glError();
        }
        else // or layered image for depth, if layered rendering is used
        {
            glGenTextures(1, &m_render);
            glError();

        }
        
    }

    glGenTextures(1, &m_texture);
    glError();

    glGenFramebuffers(1, &m_fbo);
    glError();

    resize();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();

    if(m_depth)
    {
        if (m_layerCount <= 0)
        {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render);
            glError();
        }
        else{
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_render, 0);
            glError();
        }
        
    }

	if (m_layerCount > 0)
	{
        glFramebufferTexture(GL_FRAMEBUFFER, m_attachment, m_texture, 0);
        glError();
	}
	else 
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachment
			, GL_TEXTURE_2D, m_texture, 0);
		glError();

		glDrawBuffers(1, &m_attachment);
		glError();
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
    if(m_depth && m_render != -1 && m_layerCount <= 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_render);
        glError();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_size.x, m_size.y);
        glError();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glError();
    }

    if (m_depth && m_render != -1 && m_layerCount > 0)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_render);
        glError();

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, m_size.x, m_size.y, m_layerCount, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glError();

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glError();
    }

    if(m_texture != -1)
    {
		if (m_layerCount > 0)
		{
			glBindTexture(GL_TEXTURE_3D, m_texture);
			glError();

			glTexImage3D(GL_TEXTURE_3D, 0, m_internal, m_size.x, m_size.y, m_layerCount, 0, m_format, m_type, 0);
			glError();

			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_3D, 0);
			glError();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glError();

			glTexImage2D(GL_TEXTURE_2D, 0, m_internal, m_size.x, m_size.y, 0, m_format, m_type, 0);
			glError();

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

			glBindTexture(GL_TEXTURE_2D, 0);
			glError();
		}
    }
}

void FrameBufferObject::clear()
{
    bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    release();
}
