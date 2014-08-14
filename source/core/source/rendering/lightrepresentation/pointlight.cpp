#include <core/rendering/lightrepresentation/pointlight.h>
#include <reflectionzeug/extensions/GlmProperties.hpp>


PointLight::PointLight(const glm::vec3 & intensity, const glm::vec3 & position, float radius)
: AbstractLight(intensity)
{
	setPosition(position);
	setRadius(radius);

	addProperty<glm::vec3>("Position", this, & PointLight::position, & PointLight::setPosition);
	addProperty<float>("Radius", this, & PointLight::radius, & PointLight::setRadius);
}

PointLight::~PointLight()
{
}

void PointLight::setPosition(const glm::vec3 & positionVector)
{
	m_position = positionVector;
}

void PointLight::setRadius(float radius)
{
	m_radius = radius;
}

glm::vec3 PointLight::position() const
{
	return m_position;
}

float PointLight::radius() const
{
	return m_radius;
}

void PointLight::saveLightData(LightingSystem & manager, const glm::mat4 & transform)
{
	glm::vec4 worldPosition(transform * glm::vec4(position(), 1.0f));
	manager.addPointLight(worldPosition, intensity(), radius());
}

void PointLight::draw(const Program & program, const glm::mat4 & transform)
{

}

/*const AxisAlignedBoundingBox PointLight::boundingBox() const
{
	if (m_aabb.valid())
		return m_aabb;

	m_aabb = AxisAlignedBoundingBox(position(), position());

	return m_aabb;
}


const AxisAlignedBoundingBox PointLight::boundingBox(glm::mat4 transform) const
{
	if (m_aabb.valid())
		return m_aabb;

	glm::vec4 newPosition = transform * glm::vec4(position(), 1.0f);
	newPosition /= newPosition.w;
	m_aabb = AxisAlignedBoundingBox(newPosition.xyz, newPosition.xyz);

	return m_aabb;
}*/