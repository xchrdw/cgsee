
#include <core/rendering/abstractpostprocessingstage.h>

#include <glm/mat4x4.hpp>

//#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/screenquad.h>
#include <core/painter/pipelinepainter.h>

AbstractPostProcessingStage::AbstractPostProcessingStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
{
	m_program = new Program();
}

AbstractPostProcessingStage::~AbstractPostProcessingStage(void)
{
	delete m_program;
}

void AbstractPostProcessingStage::render()
{
    bindFBO();
    m_painter.screenQuad()->draw(*m_program);
    releaseFBO();
}
