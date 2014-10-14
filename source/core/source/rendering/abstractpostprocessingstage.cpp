
#include <core/rendering/abstractpostprocessingstage.h>

#include <glm/mat4x4.hpp>

#include <globjects/Program.h>

#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/screenquad.h>
#include <core/painter/pipelinepainter.h>

AbstractPostProcessingStage::AbstractPostProcessingStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
    , m_program(new globjects::Program())
    , m_screenquad(new ScreenQuad())
{
}

AbstractPostProcessingStage::~AbstractPostProcessingStage(void)
{
	m_program->unref();
    delete m_screenquad;
}

void AbstractPostProcessingStage::render()
{
    bindFBO();
    m_screenquad->draw(*m_program);
    releaseFBO();
}
