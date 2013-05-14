#include "fpsnavigation.h"
#include "camera.h"

//glm to rotate around axis
#include "glm/gtx/rotate_vector.hpp"

FpsNavigation::FpsNavigation(Camera * camera)
: FlightNavigation(camera)
, m_inGameMode(false)
, m_lastMousePosition(glm::vec2(-1,-1))
, m_pitchAngle(0.0f)
{
    setFromMatrix(m_viewMatrix);
    m_pitchAngle= 180 - glm::degrees( std::acos(glm::dot( m_center, m_yView) / ( glm::length(m_center) * glm::length(m_yView))));
}

FpsNavigation::~FpsNavigation(void)
{
    
}

void FpsNavigation::keyPressEvent(QKeyEvent *event){
    float speed = 0.02;
    switch (event->key()) {
        case Qt::Key_W:
            forward(speed);
            break;
        case Qt::Key_S:
            forward(-speed);
            break;
        case Qt::Key_A:
            sideward(speed);
            break;
        case Qt::Key_D:
            sideward(-speed);
            break;
            
        default:
            break;
    }
}

void FpsNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FpsNavigation::mouseMoveEvent(QMouseEvent * event){
    if (m_inGameMode){
        if (m_lastMousePosition != glm::vec2(-1,-1)){
            float divX = event->x()-m_lastMousePosition.x;
            float divY = event->y()-m_lastMousePosition.y;
            
            divX *= 0.3;
            divY *= 0.3;
            pitchYaw(divX, divY);
        }
        m_lastMousePosition = glm::vec2(event->x(),event->y());
    }
}



void FpsNavigation::mousePressEvent(QMouseEvent * event){
    m_inGameMode = true;
    m_lastMousePosition = glm::vec2(-1,-1);
}

void FpsNavigation::mouseReleaseEvent(QMouseEvent * event){
    
}


void FpsNavigation::wheelEvent(QWheelEvent *event){
    
}
void FpsNavigation::pitchYaw(float pitchAngle, float yawAngle){
    pitch(pitchAngle);
    yaw(yawAngle);
}

void FpsNavigation::forward(float speed){
    glm::vec3 step = (m_zView * speed);
    m_center.x += step.x;
    m_center.z += step.z;
    m_eye.x += step.x;
    m_eye.z += step.z;
    updateView();
    updateAxis();
}

void FpsNavigation::update()
{
    // update on every frame if wasd is pressed to make smooth movements
}

void FpsNavigation::sideward(float speed){
    glm::vec3 direction = (m_xView * speed);
    m_center.x += direction.x;
    m_center.z += direction.z;
    m_eye.x += direction.x;
    m_eye.z += direction.z;
    updateView();
    updateAxis();
}

void FpsNavigation::pitch(float angle){
    if( m_pitchAngle + angle <  179.0f && m_pitchAngle + angle > 1.0f){
        m_pitchAngle += angle;
        glm::vec3 diffCenter = glm::rotate( m_center-m_eye, -angle, m_xView) - (m_center-m_eye);
        m_center += diffCenter;
        updateView();
        updateAxis();
    }
}

void FpsNavigation::yaw(float angle){
    m_center += glm::rotate( m_zView, -angle, m_yView) - m_zView;
    updateView();
    updateAxis();
}
