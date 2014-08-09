#include <core/textureobject.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

TextureObject::TextureObject()
    : m_texture(0)
    , m_internalFormat(gl::GL_RGBA_INTEGER)
    , m_format(gl::GL_RGBA)
    , m_type(gl::GL_FLOAT)
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
    gl::glFramebufferTexture2D(gl::GL_FRAMEBUFFER, attachment, gl::GL_TEXTURE_2D, m_texture, 0);
    glError();
}

void TextureObject::bindTo(gl::GLenum textureUnit)
{
    gl::glActiveTexture(textureUnit);
    gl::glBindTexture(gl::GL_TEXTURE_2D, m_texture);
}

void TextureObject::releaseFrom(gl::GLenum textureUnit)
{
    gl::glActiveTexture(textureUnit);
    gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
}

void TextureObject::generateGLTexture()
{
    gl::glGenTextures(1, &m_texture);
    glError();
    gl::glBindTexture(gl::GL_TEXTURE_2D, m_texture);
    glError();
    gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, static_cast<gl::GLint>(m_internalFormat), m_width, m_height, 0, m_format, m_type, 0);
    glError();
    gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
    gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
    gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_LINEAR));
    glError();
    gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
    glError();
}

void TextureObject::deleteGLTexture()
{
    if (m_texture != 0)
    {
        gl::glDeleteTextures(1, &m_texture);
        glError();
    }
}

