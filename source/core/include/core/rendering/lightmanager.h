#pragma once

#include <core/core_api.h>
#include "core/rendering/lighttypes.h"
#include <vector>

#include <GL/glew.h>

class CORE_API LightManager
{
public:
	LightManager();
	virtual ~LightManager();

	LightInfo m_lightInfo;
	PointLightBuffer m_pointLightBuffer;
	SpotLightBuffer m_spotLightBuffer;
	std::vector<PointLight> m_pointLights;
	std::vector<SpotLight> m_spotLights;
	//std::vector<GoboLight> m_goboLightList;

	void setDirectionalLight(glm::vec3 direction, glm::vec3 intensity);
	void disableDirectionalLight();
	void addPointLight(glm::vec4 pos, glm::vec4 color, float radius);
	void addSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float range, float conePower);

	void initBuffers();

	void updateBuffers(GLuint activeProgram);

protected:

	GLuint ubo_point; // Holds all point light structs, accessed through a function
	GLuint ubo_spot;
	GLuint ubo_info; // Holds info about how many point/spot/gobo lights there are. , also holds directional light direction + color
};