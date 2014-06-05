#include <core/rendering/ssaostage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

SsaoStage::SsaoStage(PipelinePainter & painter)
    : PostProcessingStage(painter)
{
	m_program = new Program();
}

SsaoStage::~SsaoStage(void)
{
	delete m_program;
}
