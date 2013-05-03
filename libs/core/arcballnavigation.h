#pragma once
#include "abstractnavigation.h"

class ArcballNavigation :
	public AbstractNavigation
{
public:
	ArcballNavigation(void);
	~ArcballNavigation(void);
	
	virtual void mouseMoveEvent(QMouseEvent *event);

	
	virtual const glm::mat4 viewMatrix();

private: 
	glm::vec3 center;
	glm::vec3 direction;
	float distance;
	bool invalid;

	QPoint oldPos;

};

