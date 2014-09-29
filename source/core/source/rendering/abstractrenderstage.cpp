#include <core/rendering/abstractrenderstage.h>

#include <glm/mat4x4.hpp>
#include <core/gpuquery.h>

#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/group.h>


AbstractRenderStage::AbstractRenderStage(PipelinePainter & painter)
    : m_painter(painter)
    , m_fbo(0)
{
    glGenFramebuffers(1, &m_fbo);
    glError();
}

AbstractRenderStage::~AbstractRenderStage()
{
    glDeleteFramebuffers(1, &m_fbo);
    glError();
}

PipelinePainter & AbstractRenderStage::painter()
{
    return m_painter;
}

void AbstractRenderStage::drawScene(const glm::mat4 & transform, Program * program)
{
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, transform);
    traverser.traverse(*(m_painter.scene()), drawVisitor);
}

void AbstractRenderStage::bindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();
}

void AbstractRenderStage::releaseFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glError();
}

bool AbstractRenderStage::isSceneInvalid()
{
    return m_painter.isSceneInvalid();
}

bool AbstractRenderStage::isViewInvalid()
{
    return m_painter.isViewInvalid();
}
