#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glow/Texture.h>

#include <core/program.h>
#include <core/camera/abstractcamera.h>
#include <core/fileassociatedshader.h>
#include <core/painter/pipelinepainter.h>
#include <core/renderbufferobject.h>

RenderStage::RenderStage(PipelinePainter & painter)
    : AbstractSceneRenderStage(painter)
    , m_normalz(glow::Texture::createDefault())
    , m_colorId(nullptr)
    , m_depth(new RenderBufferObject(gl::GL_DEPTH24_STENCIL8))
{

    m_painter.addTexture("normalz", m_normalz);

    m_program->attach(new FileAssociatedShader(gl::GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_program->attach(new FileAssociatedShader(gl::GL_FRAGMENT_SHADER, "data/depth_util.frag"));
    m_program->attach(new FileAssociatedShader(gl::GL_VERTEX_SHADER, "data/normalz.vert"));
}

RenderStage::~RenderStage(void)
{

}

void RenderStage::render()
{
    m_painter.camera()->setUniformsIn(*m_program);
    bindFBO();
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    drawScene(m_painter.camera()->viewProjection(), m_program);
    releaseFBO();
}
void RenderStage::reloadShaders()
{
    //TODO deprecated (glow)
}

void RenderStage::resize(const int width, const int height)
{
    //m_normalz->resize(width, height);
    m_depth->resize(width, height);
    bindFBO();
    //m_normalz->attachTo(GL_COLOR_ATTACHMENT0);
    m_depth->attachTo(gl::GL_DEPTH_STENCIL_ATTACHMENT);
    releaseFBO();
}
