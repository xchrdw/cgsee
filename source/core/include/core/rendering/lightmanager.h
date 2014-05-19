#pragma once

#include <core/rendering/lighttypes.h>
#include <list>

using namespace std;


class LightManager 
{
	LightManager();
	virtual ~LightManager();

public:	
	directionalLight m_directionalLight;
	list<pointLight>* m_pointLightList;
	list<spotLight>* m_spotLightList;
	list<goboLight>* m_goboLightList;

	void setDirectionalLight(vec3 vector);
	void addPointLight(pointLight p_pointLight);
	void addSpotLight(spotLight p_spotLight);
	void addGoboLight(goboLight p_goboLight);
};