
#include <glm/gtc/matrix_transform.hpp>

#include "painter.h"

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/datacore/abstractdata.h>
#include <core/scenegraph.h>
#include <core/objio.h>
#include <core/program.h>
#include <core/screenquad.h>


Painter::Painter()
:   AbstractPainter()
,   m_registry( nullptr )
,   m_scene(nullptr)
,   m_quad(nullptr)
,   m_normalz(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_camera(nullptr)
{
}

Painter::~Painter()
{
    delete m_scene;
    delete m_registry;
    delete m_quad;

    delete m_normalz;
    delete m_fboNormalz;
    delete m_flush;  
}

Camera * Painter::camera()
{
    return m_camera;
}

const bool Painter::initialize()
{
    AutoTimer t("Initialization of Painter");

    m_registry = new DataBlockRegistry();
    m_scene = new SceneGraph( *m_registry );

    ObjIO::groupFromObjFile( "data/suzanne.obj", *m_scene, m_scene->root() );

    if(m_scene->root().children().empty())
    {
        qWarning("Have you set the Working Directory?");
        return false;
    }
    
    glm::mat4 transform(1.f);

    transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));

    m_scene->root().setTransform(transform);

    // Camera Setup

    m_camera = new Camera();

    m_camera->setFovy (45.0f);
    m_camera->setZNear( 1.0f);
    m_camera->setZFar (10.0f);

    m_camera->append( &(m_scene->root()) );

    m_camera->setView(glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));

    m_quad = new ScreenQuad();

    // G-Buffer Shader

    m_normalz = new Program();
    m_normalz->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_normalz->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));

    // Post Processing Shader

    m_flush = new Program();
	m_flush->attach(
		new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
	m_flush->attach(
		new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));

    m_fboNormalz = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    return true;
}

void Painter::paint()
{
    AbstractPainter::paint();

    t_samplerByName sampler;

    m_camera->draw(*m_normalz, m_fboNormalz);

    sampler.clear();
    sampler["source"] = m_fboNormalz;

    bindSampler(sampler, *m_flush);
    m_quad->draw(*m_flush, nullptr);
    releaseSampler(sampler);
}

void Painter::resize(
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);
    
    m_camera->setViewport(width, height);

    m_fboNormalz->resize(width, height);
    
    postShaderRelinked();
}

void Painter::postShaderRelinked()
{
}

void Painter::bindSampler(
    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void Painter::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();
}
