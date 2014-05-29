
#include <core/rendering/scenerenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

SceneRenderStage::SceneRenderStage(PipelinePainter & painter)
    : RenderStage(painter)
{
	m_fbo = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
	m_program = new Program();
}

SceneRenderStage::~SceneRenderStage(void)
{
	delete m_program;
	delete m_fbo;
}

void SceneRenderStage::render()
{
	drawScene(m_painter.camera()->transform(), m_program, m_fbo);
}