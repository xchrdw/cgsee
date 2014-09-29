#pragma once

#include <core/core_api.h>

#include <core/typedefs.h>
#include <globjects/globjects.h>


class CORE_API TextureObject
{
public:
    TextureObject();
    TextureObject(gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type);
    TextureObject(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type);
    virtual ~TextureObject();

    bool isInitialized();

    void configure(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat, gl::GLenum format, gl::GLenum type);
    void resize(gl::GLuint width, gl::GLuint height);

    void attachTo(gl::GLenum attachment);

    void bindTo(gl::GLenum textureUnit);
    void releaseFrom(gl::GLenum textureUnit);

protected:
    void generateGLTexture();
    void deleteGLTexture();

protected:
    gl::GLuint m_texture;

    gl::GLuint m_width;
    gl::GLuint m_height;
    gl::GLenum m_internalFormat;
    gl::GLenum m_format;
    gl::GLenum m_type;

};
