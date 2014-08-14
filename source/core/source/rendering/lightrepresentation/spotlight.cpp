#include <core/rendering/lightrepresentation/spotlight.h>
#include <reflectionzeug/extensions/GlmProperties.hpp>

SpotLight::SpotLight(const glm::vec3 & intensity, const glm::vec3 & direction, float angle)
: AbstractLight(intensity)
{
	setDirection(direction);
	setConeAngle(angle);

	addProperty<glm::vec3>("Direction", this, & SpotLight::direction, & SpotLight::setDirection);
	addProperty<float>("Angle", this, & SpotLight::coneAngle, & SpotLight::setConeAngle);
}

SpotLight::~SpotLight()
{
}

void SpotLight::setDirection(const glm::vec3 & directionVector)
{
	m_direction = directionVector;
}

void SpotLight::setPosition(const glm::vec3 & positionVector)
{
	m_position = positionVector;
}

void SpotLight::setRange(float range)
{
	m_range = range;
}

void SpotLight::setConeAngle(float angle)
{
	m_coneAngle = angle;
}

glm::vec3 SpotLight::direction() const
{
	return m_direction;
}

glm::vec3 SpotLight::position() const
{
	return m_position;
}

float SpotLight::range() const
{
	return m_range;
}

float SpotLight::coneAngle() const
{
	return m_coneAngle;
}

void SpotLight::draw(const Program & program, const glm::mat4 & transform)
{

}

void SpotLight::saveLightData(LightingSystem & manager, const glm::mat4 & transform)
{
	glm::vec4 worldPosition(transform * glm::vec4(position(), 1.0f));
	manager.addSpotLight(worldPosition, direction(), intensity(), range(), coneAngle());
}

/*const AxisAlignedBoundingBox SpotLight::boundingBox() const
{
	if (m_aabb.valid())
		return m_aabb;

	m_aabb = AxisAlignedBoundingBox(position(), position());
	
	return m_aabb;
}


const AxisAlignedBoundingBox SpotLight::boundingBox(glm::mat4 transform) const
{
	if (m_aabb.valid())
		return m_aabb;

	glm::vec4 newPosition = transform * glm::vec4(position(), 1.0f);
	newPosition /= newPosition.w;
	m_aabb = AxisAlignedBoundingBox(newPosition.xyz, newPosition.xyz);

	return m_aabb;
}*/