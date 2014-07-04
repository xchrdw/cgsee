
#include <core/rendering/basicrenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>

BasicRenderStage::BasicRenderStage(PipelinePainter & painter)
    : SceneRenderStage(painter)
    , m_normalz(new TextureObject(GL_RGBA16F, GL_RGBA, GL_FLOAT))
    , m_colorId(nullptr)
    , m_depth(new RenderBufferObject(GL_DEPTH24_STENCIL8))
{

    m_painter.addTexture("normalz", m_normalz);

    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/depth_util.frag"));
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));
}

BasicRenderStage::~BasicRenderStage(void)
{

}

void BasicRenderStage::render()
{
    m_painter.camera()->setUniforms(*m_program);
    bindFBO();
    drawScene(m_painter.camera()->transform(), m_program);
    releaseFBO();
}
void BasicRenderStage::reloadShaders()
{
    //TODO deprecated (glow)
}

void BasicRenderStage::resize(const int width, const int height)
{
    m_normalz->resize(width, height);
    m_depth->resize(width, height);
    bindFBO();
    m_normalz->attachTo(GL_COLOR_ATTACHMENT0);
    m_depth->attachTo(GL_DEPTH_STENCIL_ATTACHMENT);
    releaseFBO();
}
