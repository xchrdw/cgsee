#include <core/rendering/lightrepresentation/directionallight.h>

DirectionalLight::DirectionalLight(const QString name, const glm::vec3 intensity, const glm::vec3 direction)
: AbstractLight(name, intensity)
{
	setDirection(direction);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setDirection(const glm::vec3 & directionVector)
{
	m_direction = directionVector;
}

glm::vec3 DirectionalLight::direction() const
{
	return m_direction;
}

void DirectionalLight::saveLightData(LightingSystem & manager, const glm::mat4 & transform)
{
	manager.addDirectionalLight(direction(), intensity());
}

const AxisAlignedBoundingBox DirectionalLight::boundingBox() const
{
	m_aabb = AxisAlignedBoundingBox();
	return m_aabb;
}


const AxisAlignedBoundingBox DirectionalLight::boundingBox(glm::mat4 transform) const
{
	m_aabb = AxisAlignedBoundingBox();
	return m_aabb;
}