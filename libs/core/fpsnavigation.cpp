#include "fpsnavigation.h"
#include "camera.h"

FpsNavigation::FpsNavigation(Camera * camera)
: FlightNavigation(camera)
{
    setFromMatrix(m_viewMatrix);
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
    
}



void FpsNavigation::mousePressEvent(QMouseEvent * event){
    
}

void FpsNavigation::mouseReleaseEvent(QMouseEvent * event){
    
}


void FpsNavigation::wheelEvent(QWheelEvent *event){
    
}
void FpsNavigation::yawPitch(float yawAngle, float pitchAngle){

}

void FpsNavigation::update()
{
    // update on every frame if wasd is pressed to make smooth movements
}

void FpsNavigation::sideward(float speed){
    glm::vec3 direction = (m_xView * speed);
    m_center += direction;
    m_eye += direction;
    updateView();
    updateAxis();
}
