#include <core/rendering/lightrepresentation/directionallight.h>


DirectionalLight::DirectionalLight(QString name, glm::vec3 intensity, glm::vec3 direction)
: AbstractLight(name, intensity)
{
	setDirection(direction);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setDirection(glm::vec3 directionVector)
{
	m_direction = directionVector;
}

glm::vec3 DirectionalLight::direction()
{
	return m_direction;
}