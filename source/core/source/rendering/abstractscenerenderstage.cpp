
#include <core/rendering/abstractscenerenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera/abstractcamera.h>

AbstractSceneRenderStage::AbstractSceneRenderStage(PipelinePainter & painter)
    : AbstractRenderStage(painter)
{
	m_program = new Program();
}

AbstractSceneRenderStage::~AbstractSceneRenderStage(void)
{
    delete m_program;
}

void AbstractSceneRenderStage::render()
{
    bindFBO();
    drawScene(m_painter.camera()->viewProjection(), m_program);
    releaseFBO();
}
