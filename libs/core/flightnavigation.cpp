
#include "flightnavigation.h"
#include "camera.h"

//glm to rotate around axis
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

FlightNavigation::FlightNavigation(Camera * camera) 
    : AbstractNavigation(camera)
    , m_timesKeyPressed(0)
{
    //need fix if viewmatrix isn't defined
    setFromMatrix(m_viewMatrix);
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
    m_timesKeyPressed++;
    float angle = 0.5 + m_timesKeyPressed/10.0;
    float speed = 0.05 + m_timesKeyPressed/50.0;
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
            roll(angle);
            break;
        case Qt::Key_E:
            roll(-angle);
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

void FlightNavigation::setFromMatrix(glm::mat4 view){ 
    m_fovy = 45.0f;
    m_xView = glm::vec3(0.0f ,0.0f, 0.0f);
    m_yView = glm::vec3(0.0f ,0.0f ,0.0f);
    m_zView = glm::vec3(0.0f ,0.0f ,0.0f);
    
    //inverse glm lookat
    m_yView.x = view[0][1];
    m_yView.y = view[1][1];
    m_yView.z = view[2][1];
    
    m_zView.x = - view [0][2];
    m_zView.y = - view [1][2];
    m_zView.z = - view [2][2];
    
    m_eye.x = view._inverse()[0][3];
    m_eye.y = view._inverse()[1][3];
    m_eye.z = view._inverse()[2][3];
    
    m_up = m_yView;
    m_center = glm::normalize(m_eye + m_zView);
    updateView();
 //   updateCamera();
    updateAxis();
}

void FlightNavigation::reset() {
    AbstractNavigation::reset();
    setFromMatrix(m_viewMatrix);
}

void FlightNavigation::updateAxis(){
    m_yView = m_up;
    m_zView = m_center - m_eye;
    m_xView = glm::cross(m_yView, m_zView);
}

void FlightNavigation::updateView(){
    m_viewMatrix =  glm::lookAt(m_eye, m_center, m_up);
    updateCamera();
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
    m_fovy = glm::clamp(m_fovy, 0.0f, 180.0f);
}

float FlightNavigation::getFovy(){
    return m_fovy;
}


