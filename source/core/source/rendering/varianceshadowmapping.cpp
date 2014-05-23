
#include <core/rendering/varianceshadowmapping.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>
#include <core/scenegraph/group.h>


const int ShadowMapSize = 256;

const glm::mat4 VarianceShadowMappingPass::biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

VarianceShadowMappingPass::VarianceShadowMappingPass(Camera * camera)
: DefaultPass(camera)
, m_lightCamera(nullptr)
{
	m_lightCamera = new Camera();
	m_lightCamera->setFovy(90.0);
	m_lightCamera->setZNear(camera->zNear());
	m_lightCamera->setZFar(camera->zFar());
	m_lightCamera->setViewport(glm::ivec2(ShadowMapSize, ShadowMapSize));
	m_lightCamera->setView(glm::lookAt(glm::vec3(4.0f, 5.5f, 6.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/vsm.frag"));
	m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/vsm.vert"));
}

VarianceShadowMappingPass::~VarianceShadowMappingPass()
{
	delete(m_lightCamera);
}

void VarianceShadowMappingPass::render()
{
	
}


void VarianceShadowMappingPass::setUniforms()
{

}

void VarianceShadowMappingPass::resize(const int width, const int height)
{

}