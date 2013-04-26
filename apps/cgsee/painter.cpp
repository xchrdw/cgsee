
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

// TODO
#include <glm/glm.hpp>
#include <iostream>
#include <QImage>

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

glm::mat4 gluPickMatrix2(
                GLdouble x,
                GLdouble y,
                GLdouble width,
                GLdouble height,
                const GLint viewport[4]
            )
{
    glm::mat4 M;
    GLfloat sx, sy;
    GLfloat tx, ty;

    sx = viewport[2] / width;
    sy = viewport[3] / height;
    tx = (viewport[2] + 2.0 * (viewport[0] - x)) / width;
    ty = (viewport[3] + 2.0 * (viewport[1] - y)) / height;

    M[0][0] = sx;
    M[1][0] = 0.0;
    M[2][0] = 0.0;
    M[3][0] = tx;
    M[0][1] = 0.0;
    M[1][1] = sy;
    M[2][1] = 0.0;
    M[3][1] = ty;
    M[0][2] = 0.0;
    M[1][2] = 0.0;
    M[2][2] = 1.0;
    M[3][2] = 0.0;
    M[0][3] = 0.0;
    M[1][3] = 0.0;
    M[2][3] = 0.0;
    M[3][3] = 1.0;

    return M;
}

void Painter::takeScreenshot()
{
    if(!m_camera)
        return;

    bool withAlphaChannel = false;

    std::cout << "TEST SCREENSHOT33" << std::endl;

    //const unsigned int targetWidth = 512;
    //const unsigned int targetHeight = 512;

    glm::mat4 projMatrix = m_camera->projection();
    glm::mat4 viewMatrix = m_camera->view();
    unsigned int width = m_camera->viewport().x;
    unsigned int height = m_camera->viewport().y;

    // Compute ratio
    float ratio = (float)width / (float)height;
    unsigned int targetHeight = 2048;
    unsigned int targetWidth  = (unsigned int)(targetHeight * ratio);

    // Set tile size
    unsigned int tileWidth  = 256;
    unsigned int tileHeight = 256;

    this->resize(tileWidth, tileHeight);

    QImage res( targetWidth, targetHeight, QImage::Format_RGB888 );

    GLint screenshotviewport[4]={0, 0, static_cast<GLint>(targetWidth), static_cast<GLint>(targetHeight)};

    m_camera->update();

    unsigned int numPixel = 0;
    for (unsigned int y = 0; y < targetHeight; y += tileHeight) {
        for (unsigned int x = 0; x < targetWidth; x += tileWidth) {
            glm::mat4 projMatrixNew = gluPickMatrix2(x + tileWidth/2, y + tileHeight/2, tileWidth, tileHeight, screenshotviewport) * projMatrix;

            m_camera->setTransform(projMatrixNew * viewMatrix);

            paint();

            QImage res_tile( tileWidth, tileHeight, QImage::Format_RGB888 );
            glReadPixels(0, 0, tileWidth, tileHeight, withAlphaChannel ? GL_RGB : GL_RGB, GL_UNSIGNED_BYTE, res_tile.bits());

            // Copy tile image data to our real image
            const unsigned int w = qMin(x + tileWidth, targetWidth) - x;
            const unsigned int h = qMin(y + tileHeight, targetHeight) - y;
            for (unsigned int tx = 0; tx < w; tx++)
            {
                for (unsigned int ty = 0; ty < h; ty++)
                {
                    res.setPixel(x + tx, y + ty, res_tile.pixel(tx, ty));
                }
            }
        }
    }

    this->resize(width, height);
    m_camera->setTransform(projMatrix * viewMatrix);

    res = res.mirrored(false, true); // flip vertically
    res.save("D:/test.png");

    //return true; //!progress.wasCanceled();

}
