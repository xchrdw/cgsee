
#include <glm/gtc/matrix_transform.hpp>

#include "deferredpainter.h"

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/group.h>
#include <core/objio.h>
#include <core/program.h>
#include <core/screenquad.h>


DeferredPainter::DeferredPainter()
:   AbstractPainter()
,   m_group(nullptr)
,   m_camera(nullptr)
,   m_normalz(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_edgeEnhancement(nullptr)
,   m_fboEdges(nullptr)
,   m_unsharpMaskingDepthBuffer(nullptr)
,   m_fboUnsharpMaskingDepthBuffer(nullptr)
,   m_quad(nullptr)
{
}

DeferredPainter::~DeferredPainter()
{
    delete m_group;
    delete m_quad;

    delete m_normalz;
    delete m_fboNormalz;
    delete m_edgeEnhancement;
    delete m_fboEdges;
    delete m_flush;    
}

Camera * DeferredPainter::camera()
{
    return m_camera;
}

const bool DeferredPainter::initialize()
{
    AutoTimer t("Initialization of Painter");

    m_group = ObjIO::groupFromObjFile("data/suzanne.obj");

    if(!m_group)
    {
        qWarning("Have you set the Working Directory?");
        return false;
    }

    glm::mat4 transform(1.f);

    transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));

    m_group->setTransform(transform);

    // Camera Setup

    m_camera = new Camera();

    m_camera->setFovy (45.0f);
    m_camera->setZNear( 1.0f);
    m_camera->setZFar (10.0f);

    m_camera->append(m_group);

    m_camera->setView(glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));

    m_quad = new ScreenQuad();

    // G-Buffer Shader

    m_normalz = new Program();
    m_normalz->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/gbuffers.frag"));
    m_normalz->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/gbuffers.vert"));

    // Image-space Edge Enhancement Shader

    m_edgeEnhancement = new Program();
    m_edgeEnhancement->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/edgeEnhancement.frag"));
    m_edgeEnhancement->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/ms.frag"));
    m_edgeEnhancement->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/edgeEnhancement.vert"));

    m_fboEdges = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    // Unsharp Masking the Depth Buffer Shader

    m_unsharpMaskingDepthBuffer = new Program();
    m_unsharpMaskingDepthBuffer->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/unsharpMaskingDepthBuffer.frag"));
    m_unsharpMaskingDepthBuffer->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/ms.frag"));
    m_unsharpMaskingDepthBuffer->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/unsharpMaskingDepthBuffer.vert"));

    m_fboUnsharpMaskingDepthBuffer = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    // Post Processing Shader

    m_flush = new Program();
	m_flush->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
    m_flush->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/ms.frag"));
	m_flush->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));

    m_fboNormalz = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true, true, 16);

    return true;
}

void DeferredPainter::paint()
{
    AbstractPainter::paint();

    t_samplerByName sampler;

    // 1. pass: g-buffer
    m_camera->draw(*m_normalz, m_fboNormalz);
    releaseSampler(sampler);

    // 2. pass: edge enhancement
    sampler.clear();
    sampler["g_normalDepth"] = m_fboNormalz;

    bindSampler(sampler, *m_edgeEnhancement);
    m_quad->draw(*m_edgeEnhancement, m_fboEdges);
    releaseSampler(sampler);

    // 3. pass: unsharp masking the depth buffer
    sampler.clear();
    sampler["g_normalDepth"] = m_fboNormalz;

    bindSampler(sampler, *m_unsharpMaskingDepthBuffer);
    m_quad->draw(*m_unsharpMaskingDepthBuffer, m_fboUnsharpMaskingDepthBuffer);
    releaseSampler(sampler);

    // 4. pass: combine

    sampler.clear();
    sampler["g_normalDepth"] = m_fboNormalz;
    sampler["x_edges"] = m_fboEdges;
    sampler["unsharpMaskingDepthBuffer"] = m_fboUnsharpMaskingDepthBuffer;
    bindSampler(sampler, *m_flush);

    m_quad->draw(*m_flush, nullptr);
    releaseSampler(sampler);
}

void DeferredPainter::resize(
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);
    
    m_camera->setViewport(width, height);

    m_fboNormalz->resize(width, height);
    m_fboEdges->resize(width, height);
    m_fboUnsharpMaskingDepthBuffer->resize(width, height);
    
    postShaderRelinked();
}

void DeferredPainter::postShaderRelinked()
{
}

void DeferredPainter::bindSampler(
    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void DeferredPainter::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->releaseTexture2D(slot);
}
