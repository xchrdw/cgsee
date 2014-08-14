#include <core/rendering/lightrepresentation/abstractlight.h>

#include <reflectionzeug/PropertyGroup.h>
#include <reflectionzeug/extensions/GlmProperties.hpp>
#include <reflectionzeug/StringProperty.h>


AbstractLight::AbstractLight(const glm::vec3 & intensity)
: reflectionzeug::PropertyGroup("Light")
{
	setIntensity(intensity);
	m_enable = true;

	addProperty<glm::vec3>("Color", this, & AbstractLight::intensity, & AbstractLight::setIntensity);
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
