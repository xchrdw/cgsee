#include "abstractnavigation.h"
#include "camera.h"

AbstractNavigation::AbstractNavigation(Camera *camera) 
    : m_camera(camera)
    , m_width(camera->viewport().x)
    , m_height(camera->viewport().y)
{
}


AbstractNavigation::~AbstractNavigation(void) 
{
}

void AbstractNavigation::reset()
{
    m_camera->setView(glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));
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



