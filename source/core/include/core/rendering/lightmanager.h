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
	void addPointLight(PointLight& pointLight);
	void addSpotLight(SpotLight& spotLight);

	void initBuffers();

	void updateBuffers();

protected:

	GLuint ubo_point; // Holds all point light structs, accessed through a function
	GLuint ubo_spot;
	GLuint ubo_count; // Holds info about how many point/spot/gobo lights there are. , also holds directional light direction + color
};