
#pragma once

#include "declspec.h"

#include <QString>


class CGSEE_API GPUQuery
{
public:
    static const GLint queryi(const GLenum penum);
    static const QString querys(const GLenum penum);

    static const QString glew(const GLenum penum);
    static const QString glewError(const GLenum penum);

    static const QString vendor();
    static const QString renderer();
    static const QString glewVersion();

    // Checks for an opengl error and prints to console if one occurred
    static const bool error();

    // Extension Info
    
    static const bool extensionSupported(const char *extension);

    // Memory Info (return -1 if query is not possible or failed)

    static const GLint totalMemory();
    static const GLint dedicatedMemory();
    static const GLint availableMemory();
    static const GLint evictionCount();
    static const GLint evictedMemory();

protected:
    static const GLint memoryInfo(const GLenum penum);
};
