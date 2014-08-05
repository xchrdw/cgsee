
#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera/abstractcamera.h>
#include <core/fileassociatedshader.h>

RenderStage::RenderStage(PipelinePainter & painter)
    : AbstractSceneRenderStage(painter)
    , m_normalz(new TextureObject(GL_RGBA32F, GL_RGBA, GL_FLOAT))
    , m_colorId(nullptr)
    , m_depth(new RenderBufferObject(GL_DEPTH24_STENCIL8))
{

    m_painter.addTexture("normalz", m_normalz);

    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/depth_util.frag"));
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));
}

RenderStage::~RenderStage(void)
{

}

void RenderStage::render()
{
    m_painter.camera()->setUniformsIn(*m_program);
    bindFBO();
    drawScene(m_painter.camera()->transform(), m_program);
    releaseFBO();
}
void RenderStage::reloadShaders()
{
    //TODO deprecated (glow)
}

void RenderStage::resize(const int width, const int height)
{
    m_normalz->resize(width, height);
    m_depth->resize(width, height);
    bindFBO();
    m_normalz->attachTo(GL_COLOR_ATTACHMENT0);
    m_depth->attachTo(GL_DEPTH_STENCIL_ATTACHMENT);
    releaseFBO();
}
