#include <core/rendering/lights/abstractlight.h>

#include <core/scenegraph/node.h>
#include <glm/glm.hpp>

// setter
void AbstractLight::setPosition(glm::vec3 pos)
{
	m_position = pos;
}

void AbstractLight::setDirection(glm::vec3 dir)
{
	m_direction = dir;
}

void AbstractLight::setIntensity(glm::vec3 intens)
{
	m_intensity = intens;
}

void AbstractLight::setFalloff(glm::vec3 foff)
{
	m_falloff = foff;
}

void AbstractLight::toggleOn()
{
	m_enable = true;
}
void AbstractLight::toggleOff()
{
	m_enable = false;
}

// getter
glm::vec3 AbstractLight::getPosition()
{
	return m_position;
}

glm::vec3 AbstractLight::getDirection()
{
	return m_direction;
}

glm::vec3 AbstractLight::getIntensity()
{
	return m_intensity;
}

glm::vec3 AbstractLight::getFalloff()
{
	return m_falloff;
}
};