
#include <core/textureobject.h>

#include <GL/glew.h>

TextureObject::TextureObject()
    : m_texture(0)
    , m_internalFormat(GL_RGBA_INTEGER)
    , m_format(GL_RGBA)
    , m_type(GL_FLOAT)
{

}

TextureObject::TextureObject(GLenum internalFormat, GLenum format, GLenum type)
    : m_texture(0)
    , m_internalFormat(internalFormat)
    , m_format(format)
    , m_type(type)
{

}

TextureObject::TextureObject(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type)
    : m_texture(0)
    , m_width(width)
    , m_height(height)
    , m_internalFormat(internalFormat)
    , m_format(format)
    , m_type(type)
{
    generateGLTexture();
}

TextureObject::~TextureObject()
{
    deleteGLTexture();
}

bool TextureObject::isInitialized()
{
    return m_texture != 0;
}

void TextureObject::configure(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type)
{
    m_width = width;
    m_height = height;
    m_internalFormat = internalFormat;
    m_format = format;
    m_type = type;

    deleteGLTexture();
    generateGLTexture();
}

void TextureObject::resize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;

    deleteGLTexture();
    generateGLTexture();
}

void TextureObject::attachTo(GLenum attachment)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_texture, 0);
    glError();
}

void TextureObject::bindTo(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void TextureObject::releaseFrom(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureObject::generateGLTexture()
{
    glGenTextures(1, &m_texture);
    glError();
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glError();
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glError();
    glBindTexture(GL_TEXTURE_2D, 0);
    glError();
}

void TextureObject::deleteGLTexture()
{
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
        glError();
    }
}

