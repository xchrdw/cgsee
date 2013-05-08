
#include "flightnavigation.h"
#include "camera.h"

//glm to rotate around axis
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

FlightNavigation::FlightNavigation(Camera * camera) : AbstractNavigation(camera)
{
    m_timesKeyPressed = 0;
    reset();
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
    float angle = 0.5 + m_timesKeyPressed/10;
    float speed = 0.05 + m_timesKeyPressed/50;
    switch (event->key()) {
        // Pitch, Yaw, roll
        case Qt::Key_W:
            pitch(angle);
            break;
        case Qt::Key_S:
            pitch(-angle);
            break;
        case Qt::Key_A:
            yaw(-angle);
            break;
        case Qt::Key_D:
            yaw(angle);
            break;
        case Qt::Key_Q:
            roll(-angle);
            break;
        case Qt::Key_E:
            roll(angle);
            break;

        //Move forward, backward
        case Qt::Key_Up:
            translate(speed);
            break;
        case Qt::Key_Down:
            translate(-speed);
            break;
         
        //Reset to views
        //Todo: Define Default views along different axis (like Blender)
        case Qt::Key_R:
            reset();
            break;
            
        default:
            break;
    }
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    m_timesKeyPressed = 0;
}

void FlightNavigation::wheelEvent(QWheelEvent *event){
    setFovy(event->delta());

}

void FlightNavigation::reset(){
    m_fovy = 45.0f;
    m_xView = glm::vec3(0.0f ,0.0f, 0.0f);
    m_yView = glm::vec3(0.0f ,0.0f ,0.0f);
    m_zView = glm::vec3(0.0f ,0.0f ,0.0f);
    m_eye = glm::vec3(0.0f ,0.0f , -2.0f);
    m_center = glm::vec3(0.0f ,0.0f ,0.0f);
    m_up = glm::vec3( 0.f, 1.f, 0.f);
    updateView();
    updateAxis();
}

void FlightNavigation::updateAxis(){
    m_yView = m_up;
    m_zView = m_center - m_eye;
    m_xView = glm::cross(m_yView, m_zView);
}

void FlightNavigation::updateView(){
    m_viewMatrix =  glm::lookAt(m_eye, m_center, m_up);
    m_camera->setView(m_viewMatrix);
}


const glm::mat4 FlightNavigation::viewMatrix(){
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted(){
    return glm::inverse(viewMatrix()); // TODO
}

void FlightNavigation::yaw(float angle){
    glm::vec3 diffLR = glm::rotate( m_zView, -angle, m_yView) - m_zView;
    m_center += diffLR;
    updateView();
    updateAxis();
}

void FlightNavigation::pitch(float angle){
    glm::vec3 diffCenter = glm::rotate( m_zView, -angle, m_xView) - m_zView;
    glm::vec3 diffUp = glm::rotate( m_yView, -angle, m_xView) - m_yView;
    m_center += diffCenter;
    m_up += diffUp;
    updateView();
    updateAxis();
}

void FlightNavigation::roll(float angle){
    glm::vec3 diffRotZ = glm::rotate(m_yView, angle, m_zView) - m_yView;
    m_up += diffRotZ;
    updateView();
    updateAxis();
}

void FlightNavigation::translate(float speed){
    glm::vec3 step = speed * m_zView;
    m_center += step;
    m_eye += step;
    updateView();
    updateAxis();
}

void FlightNavigation::setFovy(float fovy){
    m_fovy += fovy;
    m_fovy = glm::min(m_fovy, 175.0f);
    m_fovy = glm::max(m_fovy, 0.0f);
    std::cout << m_fovy << "\n";
}

float FlightNavigation::getFovy(){
    return m_fovy;
}


