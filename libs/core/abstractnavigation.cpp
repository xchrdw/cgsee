#include "abstractnavigation.h"


AbstractNavigation::AbstractNavigation(void)
{
}


AbstractNavigation::~AbstractNavigation(void) 
{
}

void AbstractNavigation::keyPressEvent( QKeyEvent *event ) { }


void AbstractNavigation::keyReleaseEvent( QKeyEvent *event ) { }

void AbstractNavigation::mouseMoveEvent(const glm::vec2 point) { }
void AbstractNavigation::mousePressEvent(const glm::vec2 point, MouseButton button) { }
void AbstractNavigation::mouseReleaseEvent(const glm::vec2 point, MouseButton button) { }
void AbstractNavigation::mouseDoubleClickEvent(const glm::vec2 point, MouseButton button) { }

void AbstractNavigation::wheelEvent(QWheelEvent *event) { }