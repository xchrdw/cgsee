
#include "flightnavigation.h"

FlightNavigation::FlightNavigation(void)
{
    reset();
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
    float speed = 0.05;
    switch (event->key()) {
        case Qt::Key_W:
            //go forward
            //qDebug("forward");
            m_position.z += speed;
            break;
        case Qt::Key_S:
            //go backward
            m_position.z -= speed;
            break;
        case Qt::Key_A:
            //go right
            m_position.x += speed;
            break;
        case Qt::Key_D:
            m_position.x -= speed;
            break;
        case Qt::Key_Space:
            //go up
            m_position.y += speed;
            break;
        case Qt::Key_Shift:
            //go down
            m_position.y -= speed;
            break;
            
        case Qt::Key_Left:
            m_center.x += speed;
            break;
        case Qt::Key_Right:
            m_center.x -= speed;
            break;
        case Qt::Key_Up:
            m_center.y += speed;
            break;
        case Qt::Key_Down:
            m_center.y -= speed;
            break;
            
        case Qt::Key_R:
            reset();
            break;
            
        default:
            break;
      //      qDebug(m_position.x);
    }
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FlightNavigation::reset(){
    m_position= glm::vec3(0,0,0);
    m_center = glm::vec3(0,0,0);
    m_viewMatrix = glm::lookAt(m_position + glm::vec3(0, 0, -2), m_center, glm::vec3( 0.f, 1.f, 0.f));
}

const glm::mat4 FlightNavigation::viewMatrix(){
    m_viewMatrix = glm::lookAt(m_position + glm::vec3(0, 0, -2), m_position + m_center, glm::vec3( 0.f, 1.f, 0.f));
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted()
{
    return glm::inverse(viewMatrix()); // TODO
}

