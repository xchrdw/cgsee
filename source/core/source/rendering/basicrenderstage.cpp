
#include <core/rendering/basicrenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

BasicRenderStage::BasicRenderStage(PipelinePainter & painter)
    : SceneRenderStage(painter)
{

    m_program = new Program();
}

BasicRenderStage::~BasicRenderStage(void)
{
	delete m_program;
	delete m_fbo;
}

void BasicRenderStage::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    drawScene(m_painter.camera()->transform(), m_program);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
