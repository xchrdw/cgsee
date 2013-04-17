
#include "gpuquery.h"

#include <GL/GLU.h>


// http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B


const GLint GPUQuery::totalMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

const GLint GPUQuery::dedicatedMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

const GLint GPUQuery::availableMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

const GLint GPUQuery::evictionCount()
{
    return memoryInfo(GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

const GLint GPUQuery::evictedMemory()
{
    return memoryInfo(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

const GLint GPUQuery::memoryInfo(const GLenum penum)
{
    if(!extensionSupported("GL_NVX_gpu_memory_info"))
        return -1;

    GLint memory(0); // in kb
    glGetIntegerv(penum, &memory);

    error();

    return memory;
}

const GLint GPUQuery::queryi(const GLenum penum)
{
    GLint result;
    glGetIntegerv(penum, &result);

    error();

    return result;
}

const QString GPUQuery::querys(const GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(glGetString(penum));
    error();

    return result;
}

const QString GPUQuery::vendor()
{
    return querys(GL_VENDOR);
}

const QString GPUQuery::renderer()
{
    return querys(GL_RENDERER);
}

const bool GPUQuery::error()
{
    const GLenum error(glGetError());
    const bool errorOccured(GL_NO_ERROR != error);

    if(errorOccured) 
        qCritical("OpenGL: %s", gluErrorString(error));

    return errorOccured;
}

const bool GPUQuery::extensionSupported(const char *extension)
{
    return QOpenGLContext::currentContext()->hasExtension(extension);
}
