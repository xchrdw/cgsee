
#include "flightnavigation.h"

//glm to rotate around axis
#include "glm/gtx/rotate_vector.hpp"

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
            //m_position.z += speed;
            roll(0.2);
            break;
        case Qt::Key_S:
            //go backward
            //m_position.z -= speed;
            roll(-0.2);
            break;
        case Qt::Key_A:
            //go right
            yaw(-0.2);
            break;
        case Qt::Key_D:
            //m_position.x -= speed;
            yaw(0.2);
            break;
        case Qt::Key_Space:
            //go up
            //m_position.y += speed;
            pitch(0.2);
            break;
        case Qt::Key_Shift:
            //go down
            //m_position.y -= speed;
            pitch(-0.2);
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
    m_xRotation = glm::vec3(0.0f ,0.0f, 0.0f);
    m_yRotation = glm::vec3(0.0f ,0.0f ,0.0f);
    m_zRotation = glm::vec3(0.0f ,0.0f ,0.0f);
    m_position = glm::vec3(0.0f ,0.0f , -2.0f);
    m_center = glm::vec3(0.0f ,0.0f ,0.0f);
    m_up = glm::vec3( 0.f, 1.f, 0.f);
    m_viewMatrix = glm::lookAt(m_position, m_center, m_up);
    updateRotation();
}

const glm::mat4 FlightNavigation::viewMatrix(){
    //m_viewMatrix = glm::lookAt(m_position + glm::vec3(0, 0, -2), m_position + m_center, glm::vec3( 0.f, 1.f, 0.f));
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted()
{
    return glm::inverse(viewMatrix()); // TODO
}

void FlightNavigation::yaw(float angle){
    glm::vec3 diffLR = glm::rotate( m_zRotation, -angle, m_yRotation) - m_zRotation;
    m_center += diffLR;
    m_viewMatrix =  glm::lookAt(m_position, m_center, m_up);
    updateRotation();
}

void FlightNavigation::roll(float angle){
    glm::vec3 diffRotZ = glm::rotate(m_yRotation, angle, m_zRotation) - m_yRotation;
    m_up += diffRotZ;
    m_viewMatrix =  glm::lookAt(m_position, m_center, m_up);
    updateRotation();
}

void FlightNavigation::pitch(float angle){
    glm::vec3 diffCenter = glm::rotate( m_zRotation, -angle, m_xRotation) - m_zRotation;
    glm::vec3 diffUp = glm::rotate( m_yRotation, -angle, m_xRotation) - m_yRotation;
    m_center += diffCenter;
    m_up += diffUp;
    m_viewMatrix =  glm::lookAt(m_position, m_center, m_up);
    updateRotation();
}

void FlightNavigation::updateRotation(){
    m_yRotation = m_up;
    m_zRotation = m_center - m_position;
    m_xRotation = glm::cross(m_yRotation, m_zRotation);
}


