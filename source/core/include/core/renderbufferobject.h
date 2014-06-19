#pragma once

#include <core/core_api.h>

#include <core/typedefs.h>
#include <core/gpuquery.h>


class CORE_API RenderBufferObject
{
public:
    RenderBufferObject();
    RenderBufferObject(GLenum internalFormat);
    RenderBufferObject(GLuint width, GLuint height, GLenum internalFormat);
    virtual ~RenderBufferObject();

    bool isInitialized();

    void configure(GLuint width, GLuint height, GLenum internalFormat);
    void resize(GLuint width, GLuint height);

    void attachTo(GLenum attachment);


protected:
    void generateGLRenderBuffer();
    void deleteGLRenderBuffer();

protected:
    GLuint m_buffer;

    GLuint m_width;
    GLuint m_height;
    GLenum m_internalFormat;

};
