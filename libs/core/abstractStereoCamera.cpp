#include "abstractStereoCamera.h"

#include <glm/gtc/matrix_access.hpp>
#include <QString>

AbstractStereoCamera::AbstractStereoCamera(const QString & name)
    :  Camera(name),
    m_center(glm::vec3(1,1,1)),
    m_up(glm::vec3(1,1,1)),
    m_cameraSeparation(0.1f),
    m_cameraSeparationVector(glm::vec3(1,1,1)),
    m_virtualCameraPosition(glm::vec3(1,1,1))

{
}

AbstractStereoCamera::~AbstractStereoCamera(void)
{
}
void AbstractStereoCamera::setCameraSeparation(float cameraSeparation)
{
}
void AbstractStereoCamera::setCenter(glm::vec3 center)
{
}
void AbstractStereoCamera::virtualCameraPosition(glm::vec3 viewDirection)
{
}
void AbstractStereoCamera::activateRightCamera()
{
}
void AbstractStereoCamera::activateLeftCamera()
{
}

void AbstractStereoCamera::setFromMatrix(){ 
    
    m_up = getUp();    
    m_virtualCameraPosition = getEye();    
    m_center = getCenter();    
}