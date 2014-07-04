#pragma once

#include <core/core_api.h>
#include <core/rendering/lightrepresentation/abstractlight.h>
#include <core/rendering/lightingsystem.h>

class CORE_API DirectionalLight : public AbstractLight
{

public:
	DirectionalLight(const QString name, const glm::vec3 intensity, const glm::vec3 direction);
	virtual ~DirectionalLight();

	void setDirection(const glm::vec3 & directionVector);
	glm::vec3 direction() const;

	void saveLightData(LightingSystem & manager, const glm::mat4 & transform) override;
	const AxisAlignedBoundingBox boundingBox() const override;
	const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const override;

private:
	glm::vec3 m_direction;
};