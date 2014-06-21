#include <core/rendering/lightmanager.h>

#include <list>

static const char* s_lightUniformBufferNames[LightUniformBuffers::Count] = 
{
	"PointLightBuffer",
	"SpotLightBuffer",
	"DirectionalLightBuffer",
	"LightInfoBuffer",
};

static const uint32_t s_lightUniformBufferSizes[LightUniformBuffers::Count] =
{
	sizeof(PointLight)* MAX_POINT_LIGHTS,
	sizeof(SpotLight)* MAX_SPOT_LIGHTS,
	sizeof(DirectionalLight) * MAX_SPOT_LIGHTS,
	sizeof(LightInfo)
};

LightManager::LightManager()
{
	memset(&m_lightInfo, 0, sizeof(LightInfo));
}

LightManager::~LightManager()
{
	glDeleteBuffers(LightUniformBuffers::Count, m_uniformBuffers.ubos);
}

void LightManager::initBuffers()
{
	glGenBuffers(LightUniformBuffers::Count, m_uniformBuffers.ubos);
	memcpy(m_uniformBuffers.sizes, s_lightUniformBufferSizes, sizeof(uint32_t)* LightUniformBuffers::Count);
	for (uint16_t t = 0; t < LightUniformBuffers::Count; t++)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffers.ubos[t]);
		glBufferData(GL_UNIFORM_BUFFER, m_uniformBuffers.sizes[t], nullptr, GL_DYNAMIC_DRAW);
	}
}

void LightManager::addPointLight(glm::vec4 pos, glm::vec3 intensity, float radius)
{
	PointLight light;
	light.m_position = pos;
	light.m_intensity = glm::vec4(intensity, 1.0f);
	light.m_intensity.w = 1.f;
	light.m_falloff = glm::vec4(1.f, 2.f / radius, 1.f / (radius * radius), radius); // Const, lin, exp
	m_pointLights.push_back(light);
	m_lightInfo.numPointLights = m_pointLights.size();
}

void LightManager::addSpotLight(glm::vec4 pos, glm::vec3 direction, glm::vec3 intensity, float range, float conePower)
{
	SpotLight light;
	light.m_position = pos;
	light.m_direction = glm::vec4(direction, 0.0f);
	light.m_intensity = glm::vec4(intensity, 1.0f);
	light.m_falloff = glm::vec4(1.f, 2.f / range, 1.f / (range * range), range); // Const, lin, exp
	light.m_spotlightData = glm::vec4(conePower, 0.0f, 0.0f, 0.0f);
	m_spotLights.push_back(light);
	m_lightInfo.numSpotLights = m_spotLights.size();
}

void LightManager::addDirectionalLight(glm::vec3 direction, glm::vec3 intensity)
{
	DirectionalLight light;
	light.m_direction = glm::vec4(direction, 0.0f);
	light.m_intensity = glm::vec4(intensity, 1.f);
	m_directionalLights.push_back(light);
	m_lightInfo.numDirectionalLights = m_directionalLights.size();
}

void LightManager::updateBuffer(int buffer_enum, GLuint activeProgram, void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffers.ubos[buffer_enum]);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBuffers.bindingLocations[buffer_enum], m_uniformBuffers.ubos[buffer_enum]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_uniformBuffers.sizes[buffer_enum], data);
}
void LightManager::onShaderRelink(GLuint relinkedProgram)
{
	for (uint16_t t = 0; t < LightUniformBuffers::Count; t++)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffers.ubos[t]);
		GLuint bindingLocation = glGetUniformBlockIndex(relinkedProgram, s_lightUniformBufferNames[t]);
		m_uniformBuffers.bindingLocations[t] = bindingLocation;
		glUniformBlockBinding(relinkedProgram, bindingLocation, bindingLocation);
	}
}

void LightManager::updateAllBuffers(GLuint activeProgram)
{
	// Discuss: Where to put this func...
	onShaderRelink(activeProgram);
	updateBuffer(LightUniformBuffers::Point, activeProgram, m_pointLights.data());
	updateBuffer(LightUniformBuffers::Spot, activeProgram, m_spotLights.data());
	updateBuffer(LightUniformBuffers::Directional, activeProgram, m_directionalLights.data());
	updateBuffer(LightUniformBuffers::Info, activeProgram, &m_lightInfo);
}