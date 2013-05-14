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

