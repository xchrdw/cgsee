
#include <core/rendering/abstractpostprocessingstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/screenquad.h>

AbstractPostProcessingStage::AbstractPostProcessingStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
    , m_program(new Program())
    , m_screenquad(new ScreenQuad())
{

}

AbstractPostProcessingStage::~AbstractPostProcessingStage(void)
{
	delete m_program;
    delete m_screenquad;
}

void AbstractPostProcessingStage::render()
{
    bindFBO();
    m_screenquad->draw(*m_program);
    releaseFBO();
}
