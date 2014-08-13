#pragma once

#include <core/core_api.h>

#include <glbinding/gl/types.h>

#include <QString>


class CORE_API GPUQuery
{
public:
    static const 
        int queryi(const gl::GLenum penum);
    static const QString querys(const gl::GLenum penum);

    static const QString vendor();
    static const QString renderer();
    static const QString version();

    static const bool isCoreProfile();

    // Checks for an opengl error and prints to console if one occurred
    static const bool error(
        const char * file
    ,   const int line);

    // Extension Info

    static const bool extensionSupported(const char * extension);
    static const bool extensionSupported(const QString & extension);

    // Memory Info (return -1 if query is not possible or failed)

    static const gl::GLint totalMemory();
    static const gl::GLint dedicatedMemory();
    static const gl::GLint availableMemory();
    static const gl::GLint evictionCount();
    static const gl::GLint evictedMemory();

protected:
    static const gl::GLint memoryInfo(const gl::GLint penum);
};

#define glError() GPUQuery::error(__FILE__, __LINE__)
#define glIsError() GPUQuery::error(__FILE__, __LINE__)
