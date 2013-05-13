
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
         
        //Todo: Define Default views along different axis (like Blender)
            
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
    
    //Extract Up Vector and Viewing direction from viewmatrix
    m_up.x = view[0][1];
    m_up.y = view[1][1];
    m_up.z = view[2][1];
    
    glm::vec3 direction;
    direction.x = - view [0][2];
    direction.y = - view [1][2];
    direction.z = - view [2][2];
    
    //Get Camera position (from: http://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix )
    
    glm::mat4 modelViewT = glm::transpose(view);
    
    // Get plane normals
    glm::vec3 n1(modelViewT[0]);
    glm::vec3 n2(modelViewT[1]);
    glm::vec3 n3(modelViewT[2]);
    
    // Get plane distances
    float d1(modelViewT[0].w);
    float d2(modelViewT[1].w);
    float d3(modelViewT[2].w);
    
    // Get the intersection of these 3 planes
    // (uisng math from RealTime Collision Detection by Christer Ericson)
    glm::vec3 n2n3 = glm::cross(n2, n3);
    float denom = glm::dot(n1, n2n3);
    
    m_eye = (n2n3 * d1) + glm::cross(n1, (d3*n2) - (d2*n3));
    m_eye /= -denom;
    
    m_center = m_eye + direction;
    
    updateView();
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


