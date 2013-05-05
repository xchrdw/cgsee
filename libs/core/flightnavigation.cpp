
#include "flightnavigation.h"

FlightNavigation::FlightNavigation(void)
{
    reset();
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_W:
            //go forward
            qDebug("forward");
            break;
        case Qt::Key_S:
            //go backward
            break;
        case Qt::Key_A:
            //go right
            break;
        case Qt::Key_D:
            break;
        case Qt::Key_Space:
            //go up
            break;
        case Qt::Key_Shift:
            //go down
            break;
        default:
            break;
    }
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FlightNavigation::reset(){
    m_center= glm::vec3(0,0,0);
    m_viewMatrix = glm::lookAt(m_center + glm::vec3(0, 0, -2), m_center, glm::vec3( 0.f, 1.f, 0.f));
}

const glm::mat4 FlightNavigation::viewMatrix(){
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted()
{
    return glm::inverse(viewMatrix()); // TODO
}

