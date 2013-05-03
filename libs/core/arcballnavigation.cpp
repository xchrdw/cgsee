#include "arcballnavigation.h"


ArcballNavigation::ArcballNavigation(void)
{
    reset();
}


void ArcballNavigation::reset()
{
    center = glm::vec3(0, 0, 0);
    direction = glm::vec3(1,0,0);
    distance = 0.2;
    invalid = true;
}


ArcballNavigation::~ArcballNavigation(void)
{
}

const glm::mat4 ArcballNavigation::viewMatrix() {
	if(invalid) {
		viewmatrix = glm::lookAt(center+(direction*distance), center, glm::vec3(0.0f, 1.0f, 0.0f));
		invalid = false;
	}
	return viewmatrix;
}

void ArcballNavigation::mouseMoveEvent(const glm::vec2 point)
{
	//if(event->mouseState)

	//QPoint delta = oldPos - event->pos();
	//oldPos = event->pos;
	//glm::rotate()
}
