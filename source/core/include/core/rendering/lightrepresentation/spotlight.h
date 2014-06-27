#pragma once

#include <core/rendering/lightrepresentation/abstractlight.h>

class CORE_API SpotLight : public AbstractLight
{

public:
	SpotLight(QString name, glm::vec3 intensity, glm::vec3 direction, float angle);
	virtual ~SpotLight();

	void setDirection(glm::vec3 directionVector);
	void setPosition(glm::vec3 positionVector);
	void setFalloff(glm::vec4 falloff);
	void setConeAngle(float angle);

	glm::vec3 direction();
	glm::vec3 position();
	glm::vec4 falloff();
	float coneAngle();

private:
	glm::vec3 m_direction;
	glm::vec3 m_position;
	glm::vec4 m_falloff;
	float m_coneAngle;
};