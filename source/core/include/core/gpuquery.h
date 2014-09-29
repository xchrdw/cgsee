//#pragma once
//
//#include <core/core_api.h>
//
////#include <GL/glew.h>
//#include <globjects/globjects.h>
//#include <globjects/memory.h>
//#include <globjects/Query.h>
//
//#include <QString>
//
//class CORE_API GPUQuery
//{
//public:
//    static const QString querys(const gl::GLenum penum);
//    static const QString glew  (const gl::GLenum penum);
//
//    static const QString vendor();
//    static const QString renderer();
//    static const QString version();
//    static const QString glewVersion();
//
//    static const bool isCoreProfile();
//
//    // Checks for an opengl error and prints to console if one occurred
//    static const bool error(
//        const char * file
//    ,   const int line);
//
//    static const QString glewError(const gl::GLenum penum);
//
//    // Extension Info
//
//    static const bool extensionSupported(const char * extension);
//    static const bool extensionSupported(const QString & extension);
//
//    // Memory Info (return -1 if query is not possible or failed)
//
//    static const gl::GLint totalMemory();
//    static const gl::GLint dedicatedMemory();
//    static const gl::GLint availableMemory();
//    static const gl::GLint evictionCount();
//    static const gl::GLint evictedMemory();
//
//};
//
//#define glError() GPUQuery::error(__FILE__, __LINE__)
//#define glIsError() GPUQuery::error(__FILE__, __LINE__)
