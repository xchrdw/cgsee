#include <core/rendering/lightrepresentation/abstractlight.h>


AbstractLight::AbstractLight(const QString & name, const glm::vec3 & intensity)
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

void AbstractLight::setIntensity(const glm::vec3 & rgbVector)
{
	m_intensity = rgbVector;
}

glm::vec3 AbstractLight::intensity() const
{
	return m_intensity;
}

void AbstractLight::draw(const Program & program, const glm::mat4 & transform)
{

}