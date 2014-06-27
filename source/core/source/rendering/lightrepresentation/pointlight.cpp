#include <core/rendering/lightrepresentation/pointlight.h>


PointLight::PointLight(QString name, glm::vec3 intensity, glm::vec3 position, glm::vec4 falloff)
: AbstractLight(name, intensity)
{
	setPosition(position);
	setFalloff(falloff);
}

PointLight::~PointLight()
{
}

void PointLight::setPosition(glm::vec3 positionVector)
{
	m_position = positionVector;
}

void PointLight::setFalloff(glm::vec4 falloff)
{
	m_falloff = falloff;
}

glm::vec3 PointLight::position()
{
	return m_position;
}

glm::vec4 PointLight::falloff()
{
	return m_falloff;
}