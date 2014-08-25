
#include <core/rendering/abstractscenerenderstage.h>

#include <glm/mat4x4.hpp>

#include <globjects/program.h>

#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera/abstractcamera.h>
#include <core/painter/pipelinepainter.h>

AbstractSceneRenderStage::AbstractSceneRenderStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
{
	m_program = new glo::Program();
}

AbstractSceneRenderStage::~AbstractSceneRenderStage(void)
{

}

void AbstractSceneRenderStage::render()
{
    bindFBO();
    drawScene(m_painter.camera()->viewProjection(), m_program);
    releaseFBO();
}
