
#include <core/textureobject.h>skype

TextureObject::TextureObject()
    : m_texture(0)
    , m_internalFormat(gl::GLenum::GL_RGBA_INTEGER)
    , m_format(gl::GLenum::GL_RGBA)
    , m_type(gl::GLenum::GL_FLOAT)
{

}

TextureObject::TextureObject(gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type)
    : m_texture(0)
    , m_internalFormat(internalFormat)
    , m_format(format)
    , m_type(type)
{

}

TextureObject::TextureObject(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type)
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

void TextureObject::configure(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type)
{
    m_width = width;
    m_height = height;
    m_internalFormat = internalFormat;
    m_format = format;
    m_type = type;

    deleteGLTexture();
    generateGLTexture();
}

void TextureObject::resize(gl::GLuint width, gl::GLuint height)
{
    m_width = width;
    m_height = height;

    deleteGLTexture();
    generateGLTexture();
}

void TextureObject::attachTo(gl::GLenum attachment)
{
    gl::glFramebufferTexture2D(gl::GLenum::GL_FRAMEBUFFER, attachment, gl::GLenum::GL_TEXTURE_2D, m_texture, 0);

}

void TextureObject::bindTo(gl::GLenum textureUnit)
{
    gl::glActiveTexture(textureUnit);
    gl::glBindTexture(gl::GLenum::GL_TEXTURE_2D, m_texture);
}

void TextureObject::releaseFrom(gl::GLenum textureUnit)
{
    gl::glActiveTexture(textureUnit);
    gl::glBindTexture(gl::GLenum::GL_TEXTURE_2D, 0);
}

void TextureObject::generateGLTexture()
{
    gl::glGenTextures(1, &m_texture);

    gl::glBindTexture(gl::GLenum::GL_TEXTURE_2D, m_texture);

    gl::glTexImage2D(gl::GLenum::GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0);

    gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_WRAP_S, gl::GLenum::GL_CLAMP_TO_EDGE);
    gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_WRAP_T, gl::GLenum::GL_CLAMP_TO_EDGE);
    gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_MAG_FILTER, gl::GLenum::GL_LINEAR);
    gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_MIN_FILTER, gl::GLenum::GL_LINEAR);

    gl::glBindTexture(gl::GLenum::GL_TEXTURE_2D, 0);

}

void TextureObject::deleteGLTexture()
{
    if (m_texture != 0)
    {
        gl::glDeleteTextures(1, &m_texture);
    
    }
}

