#include "abstractnavigation.h"


AbstractNavigation::AbstractNavigation(void)
{
}


AbstractNavigation::~AbstractNavigation(void) 
{
}

void AbstractNavigation::keyPressEvent( QKeyEvent *event ) { }

void AbstractNavigation::keyReleaseEvent( QKeyEvent *event ) { }

void AbstractNavigation::mouseMoveEvent(QMouseEvent * event) { }
void AbstractNavigation::mousePressEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseReleaseEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseDoubleClickEvent(QMouseEvent * event) { }

void AbstractNavigation::wheelEvent(QWheelEvent *event) { }