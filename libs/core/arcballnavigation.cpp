#include "arcballnavigation.h"


ArcballNavigation::ArcballNavigation(void)
{
}


ArcballNavigation::~ArcballNavigation(void)
{
}

const glm::mat4 ArcballNavigation::viewMatrix() {
	if(invalid) {
		viewmatrix = glm::lookAt(center+(direction*distance), center, glm::vec3(0.0f,1.0f,0.0f));
		invalid = false;
	}
	return viewmatrix;
}

void ArcballNavigation::mouseMoveEvent( QMouseEvent *event )
{
	if(event->mouseState)

	QPoint delta = oldPos - event->pos();
	oldPos = event->pos;
	glm::rotate()
}
