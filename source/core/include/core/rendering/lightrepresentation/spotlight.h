#pragma once

#include <core/core_api.h>
#include <core/rendering/lightrepresentation/abstractlight.h>
#include <core/rendering/lightingsystem.h>

class CORE_API SpotLight : public AbstractLight
{

public:
	SpotLight(const glm::vec3 & intensity, const glm::vec3 & direction, float angle);
	virtual ~SpotLight();

	void setDirection(const glm::vec3 & directionVector);
	void setPosition(const glm::vec3 & positionVector);
	void setRange(float range);
	void setConeAngle(float angle);

	glm::vec3 direction() const;
	glm::vec3 position() const;
	float range() const;
	float coneAngle() const;

	void saveLightData(LightingSystem & manager, const glm::mat4 & transform) override;
	void draw(const Program & program, const glm::mat4 & transform) override;
	/*const AxisAlignedBoundingBox boundingBox() const override;
	const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const override;*/

private:
	glm::vec3 m_direction;
	glm::vec3 m_position;
	float m_range;
	float m_coneAngle;
};