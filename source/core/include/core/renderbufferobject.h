#pragma once

#include <glbinding/gl/types.h>

#include <core/core_api.h>

#include <core/typedefs.h>
#include <core/gpuquery.h>


class CORE_API RenderBufferObject
{
public:
    RenderBufferObject();
    RenderBufferObject(gl::GLenum internalFormat);
    RenderBufferObject(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat);
    virtual ~RenderBufferObject();

    bool isInitialized();

    void configure(gl::GLuint width, gl::GLuint height, gl::GLenum internalFormat);
    void resize(gl::GLuint width, gl::GLuint height);

    void attachTo(gl::GLenum attachment);


protected:
    void generateGLRenderBuffer();
    void deleteGLRenderBuffer();

protected:
    gl::GLuint m_buffer;

    gl::GLuint m_width;
    gl::GLuint m_height;
    gl::GLenum m_internalFormat;
};
