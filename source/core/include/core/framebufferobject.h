#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <core/typedefs.h>


class Program;

class CORE_API FrameBufferObject
{
public:
    FrameBufferObject(
        const GLenum internalFormat
    ,   const GLenum format
    ,   const GLenum type
    ,   const GLenum attachment
    ,   const bool   depth);

	FrameBufferObject(
		const GLenum internalFormat
		, const GLenum format
		, const GLenum type
		, const bool   depth
		, const GLsizei layerCount);

    virtual ~FrameBufferObject();

    void bind() const;
    void release() const;

    void bindTexture2D(
        const Program & program
    ,   const QString & uniform
    ,   const glm::uint slot) const;

	void bindTexture3D(
		const Program & program
	,	const QString & uniform
	,	const glm::uint slot) const;

    void releaseTexture2D() const;
	void releaseTexture3D() const;

    void clear();

    void resize(
        const unsigned int width
    ,   const unsigned int height);

    unsigned int width();
    unsigned int height();

protected:
    void initialize() const;
    void resize() const;

    inline const bool isFrameBuffer() const;
    inline const bool isRenderBuffer() const;
    inline const bool isTexture() const;
public:
    mutable GLuint m_fbo;
protected:
    mutable GLuint m_render;
    mutable GLuint m_texture;

    glm::ivec2 m_size;

    const GLenum m_internal;
    const GLenum m_format;
    const GLenum m_type;
    const GLenum m_attachment;
	GLenum *m_multiAttachment;
    const bool   m_depth;

    unsigned int m_width;
    unsigned int m_height;

	GLsizei m_layerCount;
};
