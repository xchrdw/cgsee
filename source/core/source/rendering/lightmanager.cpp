#include <core/rendering/lightmanager.h>
#include <core/rendering/lighttypes.h>

#include <list>

class LightManager
{
	LightManager();
	virtual ~LightManager();

public:
	directionalLight m_directionalLight;
	list<pointLight> m_pointLightList;
	list<spotLight> m_spotLightList;
	list<goboLight> m_goboLightList;

	void setDirectionalLight(vec3 vector);
	void addPointLight(pointLight* p_pointLight);
	void addSpotLight(spotLight* p_spotLight);
	void addGoboLight(goboLight* p_goboLight);

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::setDirectionalLight(vec3 vector)
{
	m_directionalLight.m_direction = vector;
}

void addPointLight(pointLight* p_pointLight)
{
	m_pointLightList.push_front(&p_pointLight);
}

void addSpotLight(spotLight* p_spotLight)
{
	m_spotLightList.push_front(&p_spotLight);
}

void addGoboLight(goboLight* p_goboLight)
{
	m_goboLightList.push_front(&p_goboLight);
}

}