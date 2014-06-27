#pragma once

#include <core/rendering/lightrepresentation/abstractlight.h>

class CORE_API PointLight : public AbstractLight
{

public:
	PointLight(QString name, glm::vec3 intensity, glm::vec3 position, glm::vec4 falloff);
	~PointLight();

	void setPosition(glm::vec3 positionVector);
	void setFalloff(glm::vec4 falloff);

	glm::vec3 position();
	glm::vec4 falloff();

private:
	glm::vec3 m_position;
	glm::vec4 m_falloff;	// x: constant falloff, y: linaer falloff, z: quadratic falloff, w: range
};