
#include <core/rendering/scenerenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera.h>

SceneRenderStage::SceneRenderStage(PipelinePainter & painter)
    : RenderStage(painter)
{
	m_program = new Program();
}

SceneRenderStage::~SceneRenderStage(void)
{
    delete m_program;
}

void SceneRenderStage::render()
{
    bindFBO();
    drawScene(m_painter.camera()->transform(), m_program);
    releaseFBO();
}
