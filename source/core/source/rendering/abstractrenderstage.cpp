#include <core/rendering/abstractrenderstage.h>

#include <glm/mat4x4.hpp>
#include <glbinding/glbinding.h>
#include <globjects/globjects.h>

#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/group.h>


AbstractRenderStage::AbstractRenderStage(PipelinePainter & painter)
    : m_painter(painter)
    , m_fbo(0)
{
    gl::glGenFramebuffers(1, &m_fbo);

}

AbstractRenderStage::~AbstractRenderStage()
{
    gl::glDeleteFramebuffers(1, &m_fbo);

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
    gl::glBindFramebuffer(gl::GLenum::GL_FRAMEBUFFER, m_fbo);

}

void AbstractRenderStage::releaseFBO()
{
    gl::glBindFramebuffer(gl::GLenum::GL_FRAMEBUFFER, 0);

}

bool AbstractRenderStage::isSceneInvalid()
{
    return m_painter.isSceneInvalid();
}

bool AbstractRenderStage::isViewInvalid()
{
    return m_painter.isViewInvalid();
}
