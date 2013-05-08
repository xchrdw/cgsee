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

void AbstractNavigation::setViewPort( const int width, const int height )
{
    m_width = width;
    m_height = height;
}

void AbstractNavigation::setFovy(float fovy) { }
float AbstractNavigation::getFovy(){
    //Return Standard FOV if Navigation hasn't implemented a FOV manipulator
    return 45.0f;
}