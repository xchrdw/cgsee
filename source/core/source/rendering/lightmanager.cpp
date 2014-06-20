#include <core/rendering/lightmanager.h>

#include <list>

LightManager::LightManager() : ubo_point(0xffffffff), ubo_spot(0xffffffff), ubo_info(0xffffffff)
{
	memset(&m_lightInfo, 0, sizeof(LightInfo));
	memset(&m_pointLightBuffer, 0, sizeof(PointLightBuffer));
	memset(&m_spotLightBuffer, 0, sizeof(SpotLightBuffer));
}

LightManager::~LightManager()
{
	if (ubo_point != 0xffffffff)
		glDeleteBuffers(1, &ubo_point);
	if (ubo_spot != 0xffffffff)
		glDeleteBuffers(1, &ubo_spot);
	if (ubo_info != 0xffffffff)
		glDeleteBuffers(1, &ubo_info);
}

void LightManager::initBuffers()
{
	GLuint buffers[3];
	glGenBuffers(3, buffers);
	ubo_point = buffers[0];
	ubo_info = buffers[1];
	ubo_spot = buffers[2];

	// Bind buffers to create UBOs.
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightBuffer), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_info);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightInfo), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_spot);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightBuffer), nullptr, GL_DYNAMIC_DRAW);
}

void LightManager::addPointLight(glm::vec4 pos, glm::vec4 color, float radius)
{
	PointLight light;
	light.m_position = pos;
	light.m_intensity = color;
	light.m_intensity.w = 1.f;
	light.m_falloff = glm::vec4(1.f, 2.f / radius, 1.f / (radius * radius), 1.0); // Const, lin, exp
	m_pointLights.push_back(light);
}

void LightManager::addSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float range, float conePower)
{
	SpotLight light;
	light.m_position = pos;
	light.m_direction = direction;
	light.m_intensity = color;
	light.m_falloff = glm::vec3(1.f, 2.f / range, 1.f / (range * range)); // Const, lin, exp
	light.m_spotFalloffPower = conePower;
	light.m_dist = range;
	m_spotLights.push_back(light);
}

void LightManager::disableDirectionalLight()
{
	m_lightInfo.directionalLight.m_intensity = glm::vec3(0.0f);
}

void LightManager::setDirectionalLight(glm::vec3 direction, glm::vec3 intensity)
{
	m_lightInfo.directionalLight.m_direction = direction;
	m_lightInfo.directionalLight.m_intensity = intensity;
}

/*
	This is where things get a little interesting maybe so here's the explanation:
	The buffers which are updated to the GPU are m_pointLightBuffer and m_spotLightBuffer
	and m_lightInfo which hold point, spot and lightcount+directional light data respectively.
*/

void LightManager::updateBuffers(GLuint activeProgram)
{
	m_lightInfo.numPointLights = m_pointLights.size();
	m_lightInfo.numSpotLights = m_spotLights.size();
	GLint uniformBlockSize;

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_info);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightInfo), &m_lightInfo);
	GLuint bindingLocation = glGetUniformBlockIndex(activeProgram, "LightInfo");
	int size = sizeof(LightInfo);
	glGetActiveUniformBlockiv(activeProgram, bindingLocation,GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	glUniformBlockBinding(activeProgram, bindingLocation, bindingLocation);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, ubo_info);

	for (int t = 0; t < m_pointLights.size(); t++)
	{
		m_pointLightBuffer.lights[t].m_position = m_pointLights[t].m_position;
		m_pointLightBuffer.lights[t].m_falloff = m_pointLights[t].m_falloff;
		m_pointLightBuffer.lights[t].m_intensity = m_pointLights[t].m_intensity;
 	}

	for (int t = 0; t < m_spotLights.size(); t++)
	{
		m_spotLightBuffer.lights[t].m_position = m_spotLights[t].m_position;
		m_spotLightBuffer.lights[t].m_falloff = m_spotLights[t].m_falloff;
		m_spotLightBuffer.lights[t].m_intensity = m_spotLights[t].m_intensity;
		m_spotLightBuffer.lights[t].m_spotFalloffPower = m_spotLights[t].m_spotFalloffPower;
		m_spotLightBuffer.lights[t].m_direction = m_spotLights[t].m_direction;
		m_spotLightBuffer.lights[t].m_dist = m_spotLights[t].m_dist;
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	bindingLocation = glGetUniformBlockIndex(activeProgram, "PointLights");
	// TODO: This only needs to be done once.
	https://www.opengl.org/sdk/docs/man3/xhtml/glUniformBlockBinding.xml
	glUniformBlockBinding(activeProgram, bindingLocation, bindingLocation);
	glGetActiveUniformBlockiv(activeProgram, bindingLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, uniformBlockSize, m_pointLightBuffer.lights);
	size = sizeof(PointLightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, ubo_point);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_spot);
	bindingLocation = glGetUniformBlockIndex(activeProgram, "SpotLights");
	glUniformBlockBinding(activeProgram, bindingLocation, bindingLocation);
	glGetActiveUniformBlockiv(activeProgram, bindingLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, uniformBlockSize, m_spotLightBuffer.lights);
	size = sizeof(SpotLightBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_spot);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, ubo_spot);
}