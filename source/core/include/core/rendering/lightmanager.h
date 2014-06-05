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
	std::vector<PointLight> m_pointLightList;
	std::vector<SpotLight> m_spotLightList;
	//std::vector<GoboLight> m_goboLightList;

	void setDirectionalLight(DirectionalLight& directionalLight);
	void addPointLight(PointLight& light);
	void addPointLight(glm::vec3 pos, glm::vec3 color, float radius);
	void addSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 intensity, float dist);

	void initBuffers();

	void updateBuffers(GLuint activeProgram);

protected:

	GLuint ubo_point; // Holds all point light structs, accessed through a function
	GLuint ubo_spot;
	GLuint ubo_info; // Holds info about how many point/spot/gobo lights there are. , also holds directional light direction + color
};