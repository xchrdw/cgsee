#include "fpsnavigation.h"

FPSNavigation::FPSNavigation(Camera * camera)
: FlightNavigation(camera)
{
    setFromMatrix(m_viewMatrix);
}

FPSNavigation::~FPSNavigation(void)
{
    
}

void FPSNavigation::keyPressEvent(QKeyEvent *event){
    
}

void FPSNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FPSNavigation::mouseMoveEvent(QMouseEvent * event){
    
}

void FPSNavigation::mousePressEvent(QMouseEvent * event){
    
}

void FPSNavigation::mouseReleaseEvent(QMouseEvent * event){
    
}

void FPSNavigation::wheelEvent(QWheelEvent *event){
    
}
void FPSNavigation::yawPitch(float yawAngle, float pitchAngle){
    
}

void FPSNavigation::update()
{
    // update on every frame if wasd is pressed to make smooth movements
}
