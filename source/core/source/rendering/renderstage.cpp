
#include <core/rendering/renderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

RenderStage::RenderStage(PipelinePainter & painter)
    : m_painter(painter)
    , m_fbo(0)
{
    glGenFramebuffers(1, &m_fbo);
    glError();
}

RenderStage::~RenderStage(void)
{
    glDeleteFramebuffers(1, &m_fbo);
    glError();
}

void RenderStage::drawScene(glm::mat4 & transform, Program * program)
{
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, transform);
    traverser.traverse(*(m_painter.scene()), drawVisitor);
}

GLuint RenderStage::bindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glError();
}

void RenderStage::releaseFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glError();
}

bool RenderStage::isSceneInvalid()
{
    return m_painter.isSceneInvalid();
}

bool RenderStage::isViewInvalid()
{
    return m_painter.isViewInvalid();
}
