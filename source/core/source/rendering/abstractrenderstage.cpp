#include <core/rendering/abstractrenderstage.h>

#include <glm/mat4x4.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/Program.h>

#include <core/gpuquery.h>

#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/group.h>
#include <core/painter/pipelinepainter.h>


AbstractRenderStage::AbstractRenderStage(PipelinePainter & painter)
    : m_painter(painter)
    , m_fbo(new globjects::Framebuffer())
{

}

AbstractRenderStage::~AbstractRenderStage(void)
{
    m_fbo->unref();
}

void AbstractRenderStage::drawScene(const glm::mat4 & transform, globjects::Program * program)
{
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, transform);
    traverser.traverse(*(m_painter.scene()), drawVisitor);
}

void AbstractRenderStage::bindFBO()
{
    m_fbo->bind(gl::GL_FRAMEBUFFER);
}

void AbstractRenderStage::releaseFBO()
{
    m_fbo->unbind(gl::GL_FRAMEBUFFER);
}

bool AbstractRenderStage::isSceneInvalid()
{
    return m_painter.isSceneInvalid();
}

bool AbstractRenderStage::isViewInvalid()
{
    return m_painter.isViewInvalid();
}
