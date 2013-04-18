
#pragma once

#include <GL/glew.h>

#include <QString>

#include "declspec.h"


class CGSEE_API GPUQuery
{
public:
    static const GLint   queryi(const GLenum penum);
    static const QString querys(const GLenum penum);
    static const QString glew  (const GLenum penum);

    static const QString vendor();
    static const QString renderer();
    static const QString glewVersion();

    // Checks for an opengl error and prints to console if one occurred
    static const bool error(
        const char * file
    ,   const int line);

    static const QString glewError(const GLenum penum);

    // Extension Info
    
    static const bool extensionSupported(const char * extension);
    static const bool extensionSupported(const QString & extension);

	static const bool isShader(const GLuint shader);
	static const bool isProgram(const GLuint program);
	static const bool isBuffer(const GLuint buffer);
    static const bool isFrameBuffer(const GLuint framebuffer);
    static const bool isTexture(const GLuint texture);
    static const bool isRenderBuffer(const GLuint renderbuffer);


    // Memory Info (return -1 if query is not possible or failed)

    static const GLint totalMemory();
    static const GLint dedicatedMemory();
    static const GLint availableMemory();
    static const GLint evictionCount();
    static const GLint evictedMemory();

protected:
    static const GLint memoryInfo(const GLenum penum);
};


#ifdef _DEBUG
    #define glError() GPUQuery::error(__FILE__, __LINE__)
#else
    #define glError() false
#endif
