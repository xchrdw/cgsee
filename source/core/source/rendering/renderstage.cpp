#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>

#include <globjects/Error.h>
#include <globjects/globjects.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Renderbuffer.h>
#include <globjects/base/File.h>

#include <core/camera/abstractcamera.h>
#include <core/gpuquery.h>
#include <core/painter/pipelinepainter.h>


RenderStage::RenderStage(PipelinePainter & painter, const QString & normalzBufferName)
    : AbstractSceneRenderStage(painter)
    , m_normalz(globjects::Texture::createDefault())
    , m_colorId(nullptr)
    , m_depth(new globjects::Renderbuffer())
{
    m_painter.addTexture(normalzBufferName, m_normalz);

    m_program->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/normalz.frag")));
    m_program->attach(new globjects::Shader(gl::GL_FRAGMENT_SHADER, new globjects::File("data/depth_util.frag")));
    m_program->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::File("data/normalz.vert")));

    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_normalz);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_STENCIL_ATTACHMENT, m_depth);

    resize(painter.camera()->viewport().x, painter.camera()->viewport().y);
}

RenderStage::~RenderStage(void)
{
}

void RenderStage::render()
{
    m_painter.setCameraUniforms(*m_program);
    m_fbo->clear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    bindFBO();
    drawScene(m_painter.camera()->viewProjection(), m_program);
    releaseFBO();
}
void RenderStage::reloadShaders()
{
    //TODO deprecated (glow)
}

void RenderStage::resize(const int width, const int height)
{
    m_normalz->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_depth->storage(gl::GL_DEPTH24_STENCIL8, width, height);
}
