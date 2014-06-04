
#include <core/rendering/varianceshadowmapping.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>
#include <core/scenegraph/group.h>


const int ShadowMapSize = 2048;

const glm::mat4 VarianceShadowMappingPass::biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

VarianceShadowMappingPass::VarianceShadowMappingPass(Camera * camera)
: DefaultPass(camera)
, m_lightCamera(nullptr)
, m_lightProgram(new Program())
, m_shadowProgram(new Program())
{
	m_lightCamera = new Camera();
	m_lightCamera->setFovy(90.0);
	m_lightCamera->setZNear(camera->zNear());
	m_lightCamera->setZFar(camera->zFar());
	m_lightCamera->setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
	m_lightCamera->setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	m_lightProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_light.frag"));
	m_lightProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_light.vert"));

	m_shadowProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_shadow.frag"));
	m_shadowProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_shadow.vert"));

	m_shadowFBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
	m_shadowFBO->resize(m_camera->viewport().x, m_camera->viewport().y);

	m_shadowmapFBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
	m_shadowmapFBO->resize(ShadowMapSize, ShadowMapSize);
}

VarianceShadowMappingPass::~VarianceShadowMappingPass()
{
	delete(m_lightCamera);
	delete(m_lightProgram);
	delete(m_shadowProgram);
	delete(m_shadowmapFBO);
	delete(m_shadowFBO);
}

//I don't understand why m_FBO is broken.
FrameBufferObject* VarianceShadowMappingPass::output()
{
	return m_shadowFBO;
}

void VarianceShadowMappingPass::render()
{
	glm::mat4x4 biasLightViewProjection = biasMatrix * m_lightCamera->transform();

	drawScene(m_lightCamera, m_lightProgram, m_shadowmapFBO);

	m_shadowProgram->setUniform("inverseViewProjection", m_camera->transformInverse());
	m_shadowProgram->setUniform("biasLightViewProjection", biasLightViewProjection);
	m_shadowmapFBO->bindTexture2D(*m_shadowProgram, "shadowmap", 0);
	drawScene(m_camera, m_shadowProgram, m_shadowFBO);
}


void VarianceShadowMappingPass::setUniforms()
{

}

void VarianceShadowMappingPass::resize(const int width, const int height)
{
	m_shadowFBO->resize(width, height);
}