
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
            m_position.z += 0.01;
            break;
        case Qt::Key_S:
            //go backward
            m_position.z -= 0.01;
            break;
        case Qt::Key_A:
            //go right
            m_position.x += 0.01;
            break;
        case Qt::Key_D:
            m_position.x -= 0.01;
            break;
        case Qt::Key_Space:
            //go up
            m_position.y += 0.01;
            break;
        case Qt::Key_Shift:
            //go down
            m_position.y -= 0.01;
            break;
        default:
            break;
    }
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FlightNavigation::reset(){
    m_position= glm::vec3(0,0,0);
    m_viewMatrix = glm::lookAt(m_position + glm::vec3(0, 0, -2), m_position, glm::vec3( 0.f, 1.f, 0.f));
}

const glm::mat4 FlightNavigation::viewMatrix(){
    m_viewMatrix = glm::lookAt(m_position + glm::vec3(0, 0, -2), m_position, glm::vec3( 0.f, 1.f, 0.f));
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted()
{
    return glm::inverse(viewMatrix()); // TODO
}

