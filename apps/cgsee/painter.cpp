
#include <glm/gtc/matrix_transform.hpp>

#include "painter.h"

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

#include <QImage>
#include <QPainter>


Painter::Painter()
:   AbstractPainter()
,   m_group(nullptr)
,   m_camera(nullptr)
,   m_normalz(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_quad(nullptr)
{
}

Painter::~Painter()
{
    delete m_group;
    delete m_quad;

    delete m_normalz;
    delete m_fboNormalz;
    delete m_flush;    
}

const bool Painter::initialize()
{
    m_group = ObjIO::groupFromObjFile("data/suzanne.obj");
    if(!m_group)
    {
        qWarning("Have you set the Working Directory?");
        return false;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 transform(1.f);

    transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    transform *= glm::rotate(glm::mat4(1.f), 100.f, glm::vec3(0.f, 1.f, 0.f));

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

	// Normals and Depth to RGBA

    m_camera->draw(m_normalz, m_fboNormalz);


    sampler.clear();
	sampler["source"] = m_fboNormalz;

	bindSampler(sampler, m_flush);
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
,	Program * program)
{
	t_samplerByName::const_iterator i(sampler.begin());
	const t_samplerByName::const_iterator iEnd(sampler.end());
	
	for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void Painter::releaseSampler(
	const t_samplerByName & sampler)
{
	t_samplerByName::const_iterator i(sampler.begin());
	const t_samplerByName::const_iterator iEnd(sampler.end());

	for(; i != iEnd; ++i)
		i.value()->releaseTexture2D();
}

const QImage Painter::capture(
    const bool alpha)
{
    if(!m_camera)
        return QImage();

    // aspect is false, since this accesses the cameras projection matrix with same aspect...
    return capture(QSize(m_camera->viewport().x, m_camera->viewport().y), false, alpha);
}

const QImage Painter::capture(
    const QSize & size
,   const bool aspect
,   const bool alpha)
{
    static const GLuint tileW(256);
    static const GLuint tileH(256);

    if(!m_camera)
    {
        qWarning("No camera for frame capture available.");
        return QImage();
    }

    const GLuint w(m_camera->viewport().x);
    const GLuint h(m_camera->viewport().y);

    const GLuint frameW = size.width();
    const GLuint frameH = size.height();

    const glm::mat4 proj(aspect ? glm::perspective(m_camera->fovy()
        , static_cast<float>(frameW) / static_cast<float>(frameH)
        , m_camera->zNear(), m_camera->zFar()) : m_camera->projection());

    const glm::mat4 view(m_camera->view());

    const glm::vec4 viewport(0, 0, frameW, frameH);

    QImage frame(frameW, frameH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);
    QImage tile(tileW, tileH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    QPainter p(&frame);

    resize(tileW, tileH);

    m_camera->update();

    for (GLuint y = 0; y < frameH; y += tileH)
    for (GLuint x = 0; x < frameW; x += tileW) 
    {
        const glm::mat4 pick = glm::pickMatrix(glm::vec2(x + tileW / 2,  y + tileH / 2),
            glm::vec2(tileW, tileH), viewport);

        const glm::mat4 projTile(pick * proj);

        m_camera->setTransform(projTile * view);

        paint();

        glReadPixels(0, 0, tileW, tileH, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tile.bits());
        p.drawImage(x, y, tile);
    }
    p.end();

    resize(w, h);
    m_camera->setTransform(proj * view);

    return frame.mirrored(false, true); // flip vertically
}