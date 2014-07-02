#include <core/rendering/lightingsystem.h>

#include <list>
#include <cstring>

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
	sizeof(DirectionalLight) * MAX_DIRECTIONAL_LIGHTS,
	sizeof(LightInfo)
};

LightingSystem::LightingSystem()
{
	memset(&m_lightInfo, 0, sizeof(LightInfo));
}

LightingSystem::~LightingSystem()
{
	glDeleteBuffers(LightUniformBuffers::Count, m_uniformBuffers.ubos);
}

void LightingSystem::initBuffers()
{
	glGenBuffers(LightUniformBuffers::Count, m_uniformBuffers.ubos);
	memcpy(m_uniformBuffers.sizes, s_lightUniformBufferSizes, sizeof(uint32_t)* LightUniformBuffers::Count);
	for (uint16_t t = 0; t < LightUniformBuffers::Count; t++)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffers.ubos[t]);
		glBufferData(GL_UNIFORM_BUFFER, m_uniformBuffers.sizes[t], nullptr, GL_DYNAMIC_DRAW);
	}
}

void LightingSystem::addPointLight(const glm::vec4& pos, const glm::vec3& intensity, float radius)
{
	PointLight light;
	light.m_position = pos;
	light.m_intensity = glm::vec4(intensity, 1.0f);
	light.m_falloff = glm::vec4(1.f, 2.f / radius, 1.f / (radius * radius), radius); // Const, lin, exp
	m_pointLights.push_back(light);
	m_lightInfo.numPointLights = m_pointLights.size();
}

void LightingSystem::addSpotLight(const glm::vec4& pos, const glm::vec3& direction, const glm::vec3& intensity, float range, float conePower)
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

void LightingSystem::addDirectionalLight(const glm::vec3& direction, const glm::vec3& intensity)
{
	DirectionalLight light;
	light.m_direction = glm::vec4(direction, 0.0f);
	light.m_intensity = glm::vec4(intensity, 1.f);
	m_directionalLights.push_back(light);
	m_lightInfo.numDirectionalLights = m_directionalLights.size();
}

void LightingSystem::updateBuffer(uint8_t buffer_enum, GLuint activeProgram, void* data)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBuffers.bindingLocations[buffer_enum], m_uniformBuffers.ubos[buffer_enum]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_uniformBuffers.sizes[buffer_enum], data);
}
void LightingSystem::onShaderRelink(GLuint relinkedProgram)
{
	for (uint16_t t = 0; t < LightUniformBuffers::Count; t++)
	{
		GLuint bindingLocation = glGetUniformBlockIndex(relinkedProgram, s_lightUniformBufferNames[t]);
		m_uniformBuffers.bindingLocations[t] = bindingLocation;
		glUniformBlockBinding(relinkedProgram, bindingLocation, bindingLocation);
	}
}

void LightingSystem::updateAllBuffers(GLuint activeProgram)
{
	updateBuffer(LightUniformBuffers::Point, activeProgram, m_pointLights.data());
	updateBuffer(LightUniformBuffers::Spot, activeProgram, m_spotLights.data());
	updateBuffer(LightUniformBuffers::Directional, activeProgram, m_directionalLights.data());
	updateBuffer(LightUniformBuffers::Info, activeProgram, &m_lightInfo);
}

void LightingSystem::bindBuffer(uint8_t buffer_enum)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBuffers.bindingLocations[buffer_enum], m_uniformBuffers.ubos[buffer_enum]);
}

void LightingSystem::bindAllBuffers()
{
	for (uint8_t t = 0; t < LightUniformBuffers::Count; t++)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBuffers.bindingLocations[t], m_uniformBuffers.ubos[t]);
	}
}
