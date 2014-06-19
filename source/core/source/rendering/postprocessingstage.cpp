
#include <core/rendering/postprocessingstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/screenquad.h>

PostProcessingStage::PostProcessingStage(PipelinePainter & painter)
    : RenderStage(painter)
{
	m_program = new Program();
}

PostProcessingStage::~PostProcessingStage(void)
{
	delete m_program;
}

void PostProcessingStage::render()
{
    bindFBO();
    m_painter.screenQuad()->draw(*m_program);
    releaseFBO();
}
