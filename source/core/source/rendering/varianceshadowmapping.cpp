
#include <core/rendering/varianceshadowmapping.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>
#include <core/scenegraph/group.h>


const int ShadowMapSize = 512;

const glm::mat4 VarianceShadowMappingPass::biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

VarianceShadowMappingPass::VarianceShadowMappingPass(Camera * camera)
: DefaultPass(camera)
, m_lightCamera(nullptr)
, m_light2Camera(nullptr)
, m_lightProgram(new Program())
, m_scene(nullptr)
 {
	m_lightCamera = new Camera();
	m_lightCamera->setFovy(90.0);
	m_lightCamera->setZNear(camera->zNear());
	m_lightCamera->setZFar(camera->zFar());
	m_lightCamera->setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
    m_lightCamera->update();

	m_light2Camera = new Camera();
	m_light2Camera->setFovy(90.0);
	m_light2Camera->setZNear(camera->zNear());
	m_light2Camera->setZFar(camera->zFar());
	m_light2Camera->setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
	m_light2Camera->update();


	m_lightProgram->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_light.frag"));
	m_lightProgram->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_light.vert"));

	m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm_shadow.frag"));
	m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm_shadow.vert"));

	//m_shadowmapFBO3D = new FrameBufferObject(GL_RG32F, GL_RG, GL_FLOAT, 4, true);
	m_shadowmapFBO3D = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, true, 4);
	m_shadowmapFBO3D->resize(ShadowMapSize, ShadowMapSize);

	//m_shadowmapFBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
	//m_shadowmapFBO->resize(ShadowMapSize, ShadowMapSize);

	//m_shadowmap2FBO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
	//m_shadowmap2FBO->resize(ShadowMapSize, ShadowMapSize);
}

VarianceShadowMappingPass::~VarianceShadowMappingPass()
{
	delete(m_lightCamera);
	delete(m_light2Camera);
	delete(m_lightProgram);
	delete(m_shadowmapFBO3D);
	//delete(m_shadowmapFBO);
	//delete(m_shadowmap2FBO);
}

void VarianceShadowMappingPass::render()
{
	glm::mat4x4 *biasLightViewProjection = new glm::mat4x4[2];
	biasLightViewProjection[0] = biasMatrix * m_lightCamera->transform();
	biasLightViewProjection[1] = biasMatrix * m_light2Camera->transform();

	//drawScene(m_lightCamera, m_lightProgram, m_shadowmapFBO);
	//m_lightProgram->invalidate();
	//drawScene(m_light2Camera, m_lightProgram, m_shadowmap2FBO);

	m_lightProgram->setUniform("useLayer", 0);
	drawScene(m_lightCamera, m_lightProgram, m_shadowmapFBO3D);
	m_lightProgram->invalidate();
	m_lightProgram->setUniform("useLayer", 1);
	drawScene(m_light2Camera, m_lightProgram, m_shadowmapFBO3D);

	m_program->setUniform("inverseViewProjection", m_camera->transformInverse());
	m_program->setUniform("biasLightViewProjection", 2, biasLightViewProjection[0]);
	//m_shadowmapFBO->bindTexture2D(*m_program, "shadowmap", 0);
	//m_shadowmap2FBO->bindTexture2D(*m_program, "shadowmap2", 1);
	m_shadowmapFBO3D->bindTexture3D(*m_program, "shadowmap3D", 0);
	drawScene(m_camera, m_program, m_fbo);
}

void VarianceShadowMappingPass::sceneChanged(Group * scene)
{
	if (m_scene)
	{
		m_lightCamera->remove(m_scene);
		m_light2Camera->remove(m_scene);
	}
	m_lightCamera->append(scene);
	m_light2Camera->append(scene);
	
    AxisAlignedBoundingBox bb = scene->boundingBox();

	m_lightCamera->setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f) + bb.center(),
		bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_light2Camera->setView(glm::lookAt(glm::vec3(-2.0f, 5.5f, -4.0f) + bb.center(),
		bb.center(), glm::vec3(0.0f, 1.0f, 0.0f)));

    m_scene = scene;
}

void VarianceShadowMappingPass::setUniforms()
{

}

void VarianceShadowMappingPass::resize(const int width, const int height)
{
	DefaultPass::resize(width, height);
	m_lightCamera->update();
	m_light2Camera->update();
    m_camera->update();
}