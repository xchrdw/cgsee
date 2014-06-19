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

#include <core/bufferobject.h>

ImagePainter::ImagePainter()
    : AbstractPainter()
    , m_quad(nullptr)
    , m_imageProgram(nullptr)
    , m_gridProgram(nullptr)
    , m_gridVao(0)
    , m_gridVertexBO(nullptr)
    , m_image(nullptr)
    , m_dirty(true)
    , m_zoom(1)
    , m_pan(0, 0)
    , m_aspect(1, 1)
    , m_viewport(0, 0)
{
}

ImagePainter::~ImagePainter()
{
    delete m_quad;
    delete m_imageProgram;
    delete m_gridProgram;
    delete m_image;
}

const bool ImagePainter::initialize()
{
    AutoTimer t("Initialization of ImagePainter");

    m_imageProgram = new Program();
    m_imageProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));
    m_imageProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/image.frag"));

    m_quad = new ScreenQuad();


    m_gridProgram = new Program();
    m_gridProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/image_grid.vert"));
    m_gridProgram->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/image_grid.geom"));
    m_gridProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/image_grid.frag"));

    // We need to send a dummy array to the GPU
    // see http://stackoverflow.com/questions/8039929/opengl-drawarrays-without-binding-vbo

    glGenVertexArrays(1, &m_gridVao);
    glError();
    glBindVertexArray(m_gridVao);
    glError();

    // setup array buffer
    m_gridVertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_gridVertexBO->data<GLfloat>(0, 0, GL_FLOAT, 2);

    // bind all buffers to their attributes
    m_gridVertexBO->bind(m_gridProgram->attributeLocation("a_vertex"));

    glBindVertexArray(0);
    glError();

    return true;
}

void ImagePainter::setUniforms()
{
    if (m_image)
    {
        m_image->bind(*m_imageProgram, "image", 0);
        m_imageProgram->setUniform("zoom", m_zoom);
        m_imageProgram->setUniform("pan", m_pan);
        m_imageProgram->setUniform("aspect", m_aspect);

        m_dirty = false;
    }
}

void ImagePainter::paint()
{
    AbstractPainter::paint();

    if (m_dirty)
        setUniforms();
    
    m_quad->draw(*m_imageProgram);

    paintGrid();
}

void ImagePainter::paintGrid()
{
    float pixelWidth = 0;
    if (m_aspect.x == 1) {
        pixelWidth = m_viewport.x / m_image->width() * m_zoom;
    }
    else {
        pixelWidth = m_viewport.y / m_image->height() * m_zoom;
    }

    if (pixelWidth < 30)
        return;

    m_gridProgram->use();
    glBindVertexArray(m_gridVao);
    glError();

    glDisable(GL_DEPTH_TEST);
    glError();
    glEnable(GL_CULL_FACE);
    glError();
    glDepthMask(GL_FALSE);
    glError();

    float lines = m_viewport.x / pixelWidth;
    m_gridProgram->setUniform("lines", lines);
    glDrawArraysInstanced(GL_POINTS, 0, 1, lines+1);
    glError();

    lines = m_viewport.y / pixelWidth;
    m_gridProgram->setUniform("lines", -lines);
    glDrawArraysInstanced(GL_POINTS, 0, 1, lines+1);
    glError();

    glDisable(GL_CULL_FACE);
    glError();
    glDepthMask(GL_TRUE);
    glError();

    glBindVertexArray(0);
    glError();
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
    m_zoom -= delta;
    m_zoom = m_zoom < 0 ? 0 : m_zoom;
    m_dirty = true;
}

void ImagePainter::setZoomFactor(float factor) {
    m_zoom *= factor;
    m_dirty = true;
}

void ImagePainter::resize(const int width, const int height) {
    AbstractPainter::resize(width, height);
    m_viewport = glm::vec2(width, height);
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