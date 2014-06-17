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
:   AbstractPainter()
,   m_quad(nullptr)
,   m_program(nullptr)
,   m_image(nullptr)
,   m_image_bound(false)
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
        m_image_bound = true;
    }
}

void ImagePainter::paint()
{
    AbstractPainter::paint();
    if (!m_image_bound)
        setUniforms();
    m_quad->draw(*m_program);
}

void ImagePainter::assignImage(Image * image) {
    delete m_image;
    m_image_bound = false;
    m_image = image;
}