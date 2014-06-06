#include <core/rendering/lightmanager.h>

#include <list>

LightManager::LightManager() : ubo_point(0xffffffff), ubo_spot(0xffffffff), ubo_info(0xffffffff)
{
	memset(&m_lightInfo, 0, sizeof(LightInfo));
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
	//glBindBuffer(GL_UNIFORM_BUFFER, ubo_spot);


	// I feel so C++11 when I use nullptr :)
	// Set buffer size and init the buffers on the GPU
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightBuffer), nullptr, GL_DYNAMIC_DRAW);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLight)* MAX_SPOT_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_info);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightInfo), nullptr, GL_DYNAMIC_DRAW);

	/*
	DYNAMIC_DRAW
	The data store contents will be respecified repeatedly by the ap-
	plication, and used many times as the source for GL drawing and image
	specification commands

	STREAM_DRAW
	The data store contents will be specified once by the application,
	and used at most a few times as the source for GL drawing and image speci-
	fication commands
	*/
}
void LightManager::addPointLight(glm::vec4 pos, glm::vec4 color, float radius)
{
	PointLight light;
	light.m_position = pos;
	light.m_intensity = color;
	light.m_intensity.w = 1.f;
	light.m_falloff = glm::vec4(1.f, 2.f / radius, 1.f / (radius * radius), 1.0); // Const, lin, exp
	m_pointLightList.push_back(light);
}

void LightManager::setDirectionalLight(DirectionalLight& directionalLight)
{
	m_lightInfo.directionalLight = directionalLight;
}

void LightManager::addPointLight(PointLight& pointLight)
{
	m_pointLightList.push_back(pointLight);
}

void LightManager::addSpotLight(SpotLight& spotLight)
{
	m_spotLightList.push_back(spotLight);
}

void LightManager::updateBuffers(GLuint activeProgram)
{
	m_lightInfo.numPointLights = m_pointLightList.size();
	m_lightInfo.numSpotLights = m_spotLightList.size();
	GLint uniformBlockSize;

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_info);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightInfo), &m_lightInfo);
	GLuint bindingLocation = glGetUniformBlockIndex(activeProgram, "LightInfo");
	int size = sizeof(LightInfo);
	glGetActiveUniformBlockiv(activeProgram, bindingLocation,GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	glUniformBlockBinding(activeProgram, bindingLocation, bindingLocation);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, ubo_info);

	for (int t = 0; t < m_pointLightList.size(); t++)
	{
		m_pointLightBuffer.lights[t].m_position = m_pointLightList[t].m_position;
		m_pointLightBuffer.lights[t].m_falloff = m_pointLightList[t].m_falloff;
		m_pointLightBuffer.lights[t].m_intensity = m_pointLightList[t].m_intensity;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	bindingLocation = glGetUniformBlockIndex(activeProgram, "PointLights");
	glUniformBlockBinding(activeProgram, bindingLocation, bindingLocation);
	glGetActiveUniformBlockiv(activeProgram, bindingLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, uniformBlockSize, m_pointLightBuffer.lights);
	size = sizeof(PointLightBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, ubo_point);
}