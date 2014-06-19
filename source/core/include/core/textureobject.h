#pragma once

#include <core/core_api.h>

#include <core/typedefs.h>
#include <core/gpuquery.h>


class CORE_API TextureObject
{
public:
    TextureObject();
    TextureObject(GLenum internalFormat, GLenum format, GLenum type);
    TextureObject(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type);
    virtual ~TextureObject();

    bool isInitialized();

    void configure(GLuint width, GLuint height, GLenum internalFormat, GLenum format, GLenum type);
    void resize(GLuint width, GLuint height);

    void attachTo(GLenum attachment);

    void bindTo(GLenum textureUnit);
    void releaseFrom(GLenum textureUnit);

protected:
    void generateGLTexture();
    void deleteGLTexture();

protected:
    GLuint m_texture;

    GLuint m_width;
    GLuint m_height;
    GLenum m_internalFormat;
    GLenum m_format;
    GLenum m_type;

};
