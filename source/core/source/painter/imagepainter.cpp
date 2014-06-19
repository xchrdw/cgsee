#include <QDebug>
#include <QWidget>

#include <core/painter/imagepainter.h>
#include <core/material/image.h>

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/program.h>
#include <core/screenquad.h>

ImagePainter::ImagePainter()
    : AbstractPainter()
    , m_quad(nullptr)
    , m_program(nullptr)
    , m_image(nullptr)
    , m_dirty(true)
    , m_zoom(1)
    , m_pan(0,0)
    , m_aspect(1,1)
{
}

ImagePainter::~ImagePainter()
{
    delete m_quad;
    delete m_program;
    delete m_image;
}

const bool ImagePainter::initialize()
{
    AutoTimer t("Initialization of ImagePainter");

    FileAssociatedShader * screenQuadShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert");
    FileAssociatedShader * imageShader = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/image.frag");

    m_program = new Program();
    m_program->attach(screenQuadShader);
    m_program->attach(imageShader);

    m_quad = new ScreenQuad();

    return true;
}

void ImagePainter::setUniforms()
{
    if (m_image)
    {
        m_image->bind(*m_program, "image", 0);
        m_program->setUniform("zoom", m_zoom);
        m_program->setUniform("pan", m_pan);
        m_program->setUniform("aspect", m_aspect);
        m_dirty = false;
    }
}

void ImagePainter::paint()
{
    AbstractPainter::paint();
    if (m_dirty)
        setUniforms();
    m_quad->draw(*m_program);
}

void ImagePainter::assignImage(Image * image) {
    delete m_image;
    m_dirty = true;
    m_image = image;
}

void ImagePainter::setPanDelta(glm::vec2 delta) {
    m_pan += delta;
    m_dirty = true;
}

float ImagePainter::getZoom() {
    return m_zoom;
}

void ImagePainter::setZoomDelta(float delta) {
    m_zoom += delta;
    m_zoom = m_zoom < 0 ? 0 : m_zoom;
    m_dirty = true;
}

void ImagePainter::setZoomFactor(float factor) {
    m_zoom *= factor;
    m_dirty = true;
}

void ImagePainter::resize(const int width, const int height) {
    AbstractPainter::resize(width, height);
    if (!m_image)
        return;

    float aX = m_image->width() / (float)width;
    float aY = m_image->height() / (float)height;

    if (aX > aY) {
        m_aspect = glm::vec2(1, aY/aX);
    }
    else {
        m_aspect = glm::vec2(aX/aY, 1);
    }

    m_dirty = true;
}