
#include <core/gpuquery.h>

//#ifdef WIN32
//#include <GL/wglew.h>
//#elif defined(__APPLE__)
//
//#else
//#include <GL/glxew.h>
//#endif

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>


// http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt

//#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
//#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
//#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
//#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
//#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B
//
//
//const gl::GLint GPUQuery::totalMemory()
//{
//    return memoryInfo(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
//}
//
//const gl::GLint GPUQuery::dedicatedMemory()
//{
//    return memoryInfo(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
//}
//
//const gl::GLint GPUQuery::availableMemory()
//{
//    return memoryInfo(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
//}
//
//const gl::GLint GPUQuery::evictionCount()
//{
//    return memoryInfo(GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
//}
//
//const gl::GLint GPUQuery::evictedMemory()
//{
//    return memoryInfo(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
//}

const gl::GLint GPUQuery::memoryInfo(const gl::GLenum penum)
{
    if(!extensionSupported("GL_NVX_gpu_memory_info"))
        return -1;

    gl::GLint memory(0); // in kb
    gl::glGetIntegerv(penum, &memory);
    glError();

    return memory;
}

const gl::GLint GPUQuery::queryi(const gl::GLenum penum)
{
    gl::GLint result;
    gl::glGetIntegerv(penum, &result);

    glError();

    return result;
}

const QString GPUQuery::querys(const gl::GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(gl::glGetString(penum));
    glError();

    return result;
}

const QString GPUQuery::glew(const gl::GLenum penum)
{
    //const QString result(reinterpret_cast<const char*>(glewGetString(penum)));
    glError();

    return QString("");
}

const QString GPUQuery::vendor()
{
    return querys(gl::GL_VENDOR);
}

const QString GPUQuery::renderer()
{
    return querys(gl::GL_RENDERER);
}

const QString GPUQuery::version()
{
    return querys(gl::GL_VERSION);
}

//const QString GPUQuery::glewVersion()
//{
//    return glew(GLEW_VERSION);
//}

//const bool GPUQuery::isCoreProfile()
//{
//    // HACK: just check for single extension. If it is
//    // not supported, its probably core profile.. :P
//
//    return !glewIsSupported("GL_EXT_abgr"); // extension with number 1 ;)
//}

//const QString GPUQuery::glewError(const gl::GLenum penum)
//{
//    const QString result = reinterpret_cast<const char*>(glewGetErrorString(penum));
//    return result;
//}

const bool GPUQuery::error(
    const char * file
,   const int line)
{
    const gl::GLenum error(gl::glGetError());
    const bool errorOccured(gl::GL_NO_ERROR != error);

    if(errorOccured) 
        qCritical("OpenGL: [0x%x %s : %i]\n", error, file, line);

    return errorOccured;
}

const bool GPUQuery::extensionSupported(const char * extension)
{
    //if(isCoreProfile())
    //    return false;
    return false;

//    bool supported = glewIsSupported(extension) ? true : false;
//
//    if(!supported)
//#ifdef WIN32
//        return wglewIsSupported(extension) ? true : false;
//#elif defined(LINUX)
//        return glxewIsSupported(extension) ? true : false;
//#else
//        return supported;
//#endif
//
//    return supported;
}

const bool GPUQuery::extensionSupported(const QString & extension)
{
    QByteArray b(extension.toLocal8Bit());
    return extensionSupported(b.constData());
}
