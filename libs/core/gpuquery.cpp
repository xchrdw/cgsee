
#include "gpuquery.h"

#ifdef WIN32
#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#endif


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

    glError();

    return memory;
}

const GLint GPUQuery::queryi(const GLenum penum)
{
    GLint result;
    glGetIntegerv(penum, &result);

    glError();

    return result;
}

const QString GPUQuery::querys(const GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(glGetString(penum));
    glError();

    return result;
}

const QString GPUQuery::glew(const GLenum penum)
{
    const QString result(reinterpret_cast<const char*>(glewGetString(penum)));
    glError();

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

const QString GPUQuery::glewVersion()
{
    return glew(GLEW_VERSION);
}

const QString GPUQuery::glewError(const GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(glewGetErrorString(penum));
    return result;
}

const bool GPUQuery::error(
    const char * file
,   const int line)
{
    const GLenum error(glGetError());
    const bool errorOccured(GL_NO_ERROR != error);

    if(errorOccured) 
        qCritical("OpenGL: %s [0x%x %s : %i]\n", glewGetErrorString(error), error, file, line);

    return errorOccured;
}

const bool GPUQuery::isShader(const GLuint shader)
{
	const bool is(glIsShader(shader) == GL_TRUE);
	if(!is)
		glGetError();

	return is;
}

const bool GPUQuery::isProgram(const GLuint program)
{
	const bool is(glIsProgram(program) == GL_TRUE);
	if(!is)
		glGetError();
	return is;
}

const bool GPUQuery::isBuffer(const GLuint buffer)
{
    const bool is(glIsBuffer(buffer) == GL_TRUE);
	if(!is)
		glGetError();
	return is;
}

const bool GPUQuery::isFrameBuffer(const GLuint framebuffer)
{
    const bool is(glIsFramebuffer(framebuffer) == GL_TRUE);
	if(!is)
		glGetError();
	return is;
}

const bool GPUQuery::isTexture(const GLuint texture)
{
    const bool is(glIsTexture(texture) == GL_TRUE);
	if(!is)
		glGetError();
	return is;
}

const bool GPUQuery::isRenderBuffer(const GLuint renderbuffer)
{
    const bool is(glIsTexture(renderbuffer) == GL_TRUE);
	if(!is)
		glGetError();
	return is;
}

const bool GPUQuery::extensionSupported(const char * extension)
{
    bool supported = glewIsSupported(extension) ? true : false;

    if(!supported)
#ifdef WIN32
        return wglewIsSupported(extension) ? true : false;
#elif defined(LINUX)
        return glxewIsSupported(extension) ? true : false;
#else
        return supported;
#endif

    return supported;
}

const bool GPUQuery::extensionSupported(const QString & extension)
{
    QByteArray b(extension.toLocal8Bit());
    return extensionSupported(b.constData());
}
