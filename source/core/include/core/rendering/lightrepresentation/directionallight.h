#pragma once

#include <core/rendering/lightrepresentation/abstractlight.h>

class CORE_API DirectionalLight : public AbstractLight
{

public:
	DirectionalLight(QString name, glm::vec3 intensity, glm::vec3 direction);
	virtual ~DirectionalLight();

	void setDirection(glm::vec3 directionVector);
	glm::vec3 direction();

private:
	glm::vec3 m_direction;
};