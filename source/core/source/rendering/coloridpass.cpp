
#include <core/rendering/coloridpass.h>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/fileassociatedshader.h>


ColorIdPass::ColorIdPass(Camera * camera)
:   RenderingPass(camera)
{
    // TODO (jg) : If the type of frame buffer object does not change ColorIdPass can inherit from DefaultPass (which would be cleaner).
    m_fbo = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_program = new Program();
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/colorId.frag"));
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/colorId.vert"));
}

ColorIdPass::~ColorIdPass(void)
{
    delete m_program;
    delete m_fbo;
}

void ColorIdPass::render()
{
    drawScene(m_camera, m_program, m_fbo);
}

void ColorIdPass::clearFbos()
{
    m_fbo->clear();
}

FrameBufferObject * ColorIdPass::output()
{
    return m_fbo;
}

void ColorIdPass::resize(const int width, const int height)
{
    m_fbo->resize(width, height);
}

void ColorIdPass::setUniforms()
{
    
}
