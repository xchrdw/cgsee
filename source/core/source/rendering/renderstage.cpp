
#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/painter/framebufferprovider.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>



RenderingPass::RenderStage(Painter &painter)
    : m_painter(painter)
{
}

RenderingPass::~RenderStage(void)
{
}

void RenderingPass::drawScene(glm::mat4 & transform, Program * program,  FrameBufferObject * fbo)
{
    fbo->bind();
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, transform);
    traverser.traverse(*(m_painter.scene()), drawVisitor);
    fbo->release();
}
