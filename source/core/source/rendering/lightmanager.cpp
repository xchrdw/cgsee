#include <core/rendering/lightmanager.h>
#include <core/rendering/lighttypes.h>

#include <list>


LightManager::LightManager() : ubo_point(0xffffffff), ubo_spot(0xffffffff), ubo_count(0xffffffff)
{
	memset(&m_lightInfo, 0, sizeof(LightInfo));
}

LightManager::~LightManager()
{
	if (ubo_point != 0xffffffff)
		glDeleteBuffers(1, &ubo_point);
	if (ubo_spot != 0xffffffff)
		glDeleteBuffers(1, &ubo_spot);
	if (ubo_count != 0xffffffff)
		glDeleteBuffers(1, &ubo_count);
}

void LightManager::initBuffers()
{
	GLuint buffers[3];
	glGenBuffers(3, buffers);
	ubo_point = buffers[0];
	ubo_count = buffers[1];
	ubo_spot = buffers[2];

	// Bind buffers to create UBOs.
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_point);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_spot);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_count);


	// I feel so C++11 when I use nullptr :)
	// Set buffer size and init the buffers on the GPU
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight)* MAX_POINT_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLight)* MAX_SPOT_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
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
