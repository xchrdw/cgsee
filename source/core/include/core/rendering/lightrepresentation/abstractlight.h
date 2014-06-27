#pragma once

#include <core/scenegraph/node.h>


class CORE_API AbstractLight : public Node
{

public:
	AbstractLight(QString name, glm::vec3 intensity);
	virtual ~AbstractLight();

	virtual void toggle() = 0;
	void setIntensity(glm::vec3 rgbVector);
	glm::vec3 intensity();

protected: 
	glm::vec3 m_intensity;
	bool m_enable;
};