
#include <core/gpuquery.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


// http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B


const gl::GLint GPUQuery::totalMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

const gl::GLint GPUQuery::dedicatedMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

const gl::GLint GPUQuery::availableMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

const gl::GLint GPUQuery::evictionCount()
{
    return memoryInfo(GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

const gl::GLint GPUQuery::evictedMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

const gl::GLint GPUQuery::memoryInfo(const gl::GLint penum)
{
    if(!extensionSupported("GL_NVX_gpu_memory_info"))
        return -1;

    gl::GLint memory(0); // in kb
    gl::glGetIntegerv(static_cast<gl::GLenum>(penum), &memory);
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

const bool GPUQuery::isCoreProfile()
{
    return globjects::isCoreProfile();
}

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
    return globjects::hasExtension(std::string(extension));
}

const bool GPUQuery::extensionSupported(const QString & extension)
{
    QByteArray b(extension.toLocal8Bit());
    return extensionSupported(b.constData());
}
