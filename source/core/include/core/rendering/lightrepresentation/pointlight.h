#pragma once

#include <core/core_api.h>
#include <core/rendering/lightrepresentation/abstractlight.h>
#include <core/rendering/lightingsystem.h>

class CORE_API PointLight : public AbstractLight
{

public:
	PointLight(const QString & name, const glm::vec3 & intensity, const glm::vec3 & position, float radius);
	~PointLight();

	void setPosition(const glm::vec3 & positionVector);
	void setRadius(float radius);

	glm::vec3 position() const;
	float radius() const; 

	void saveLightData(LightingSystem & manager, const glm::mat4 & transform) override;
	const AxisAlignedBoundingBox boundingBox() const override;
	const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const override;

private:
	glm::vec3 m_position;
	float m_radius;
};