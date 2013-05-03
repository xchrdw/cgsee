#pragma once
#include "abstractnavigation.h"

class ArcballNavigation :
	public AbstractNavigation
{
public:
	ArcballNavigation(void);
	~ArcballNavigation(void);
	
    virtual void mouseMoveEvent(const glm::vec2 point);
    virtual void reset();
	
	virtual const glm::mat4 viewMatrix();

private: 
	glm::vec3 center;
	glm::vec3 direction;
	float distance;
	bool invalid;

	QPoint oldPos;
    glm::mat4 viewmatrix;
};

