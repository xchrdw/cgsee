#include <core/rendering/lightrepresentation/abstractlight.h>


AbstractLight::AbstractLight(QString name, glm::vec3 intensity)
: Node(name)
{
	setIntensity(intensity);
	m_enable = true;
}

AbstractLight::~AbstractLight()
{
}

void AbstractLight::toggle()
{
	(m_enable == false) ? m_enable = true : m_enable = false;
}

void AbstractLight::setIntensity(glm::vec3 rgbVector)
{
	m_intensity = rgbVector;
}

glm::vec3 AbstractLight::intensity()
{
	return m_intensity;
}

