#include <core/rendering/lightrepresentation/spotlight.h>

SpotLight::SpotLight(QString name, glm::vec3 intensity, glm::vec3 direction, float angle)
: AbstractLight(name, intensity)
{
	setDirection(direction);
	setConeAngle(angle);
}

SpotLight::~SpotLight()
{
}

void SpotLight::setDirection(glm::vec3 directionVector)
{
	m_direction = directionVector;
}

void SpotLight::setPosition(glm::vec3 positionVector)
{
	m_position = positionVector;
}

void SpotLight::setFalloff(glm::vec4 falloff)
{
	m_falloff = falloff;
}

void SpotLight::setConeAngle(float angle)
{
	m_coneAngle = angle;
}

glm::vec3 SpotLight::direction()
{
	return m_direction;
}

glm::vec3 SpotLight::position()
{
	return m_position;
}

glm::vec4 SpotLight::falloff()
{
	return m_falloff;
}

float SpotLight::coneAngle()
{
	return m_coneAngle;
}

