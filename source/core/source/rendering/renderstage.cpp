
#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

RenderStage::RenderStage(PipelinePainter & painter)
    : m_painter(painter)
{

}

RenderStage::~RenderStage(void)
{

}

void RenderStage::drawScene(glm::mat4 & transform, Program * program)
{
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, transform);
    traverser.traverse(*(m_painter.scene()), drawVisitor);

}

FrameBufferObject * RenderStage::getFramebuffer(u_int32_t slot)
{
    return m_painter.getFramebuffer(slot);
}

void RenderStage::setFramebuffer(u_int32_t slot, FrameBufferObject fbo)
{
    m_painter.setFramebuffer(slot, fbo);
}

void RenderStage::unsetFramebuffer(u_int32_t slot)
{
    m_painter.unsetFramebuffer(slot);
}

bool RenderStage::isSceneInvalid()
{
    return m_painter.isSceneInvalid();
}

bool RenderStage::isViewInvalid()
{
    return m_painter.isViewInvalid();
}
