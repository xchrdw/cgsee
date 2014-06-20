#pragma once

#include <core/scenegraph/node.h>


class CORE_API AbstractLight : public Node
{	
public:

	//setter
	virtual void setPosition(glm::vec3 pos) = 0;
	void setDirection(glm::vec3 dir);
	void setIntensity(glm::vec3 intens);
	void setFalloff(glm::vec3 foff);
	void toggleOn();
	void toggleOff();

	// getter
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getIntensity();
	glm::vec3 getFalloff();

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_intensity;
	glm::vec3 m_falloff;
	bool m_enable;
};