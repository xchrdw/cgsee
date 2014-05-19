#include <core/rendering/lightmanager.h>
#include <core/rendering/lighttypes.h>

#include <list>


LightManager::LightManager()
{
	m_pointLightList = new list<pointLight>();
	m_spotLightList = new list<spotLight>();
	m_goboLightList = new list<goboLight>();
}

LightManager::~LightManager()
{
}

void LightManager::setDirectionalLight(vec3 vector)
{
	m_directionalLight.m_direction = vector;
}

void addPointLight(pointLight p_pointLight)
{
	m_pointLightList->push_front(p_pointLight);
}

void addSpotLight(spotLight p_spotLight)
{
	m_spotLightList->push_front(p_spotLight);
}

void addGoboLight(goboLight p_goboLight)
{
	m_goboLightList->push_front(p_goboLight);
}