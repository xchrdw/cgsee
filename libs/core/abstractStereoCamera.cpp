#include "abstractStereoCamera.h"

#include <QString>

AbstractStereoCamera::AbstractStereoCamera(const QString & name)
    :  Camera(name),
    m_center(glm::vec3(1,1,1)),
    m_cameraSeparation(glm::vec3(1,1,1)),
    m_viewDirection(glm::vec3(1,1,1))
{
}

AbstractStereoCamera::~AbstractStereoCamera(void)
{
}
void AbstractStereoCamera::setCameraSeparation(glm::vec3 cameraSeparation)
{
}
void AbstractStereoCamera::setCenter(glm::vec3 center)
{
}
void AbstractStereoCamera::setViewDirection(glm::vec3 viewDirection)
{
}
void AbstractStereoCamera::activateRightCamera()
{
}
void AbstractStereoCamera::activateLeftCamera()
{
}
