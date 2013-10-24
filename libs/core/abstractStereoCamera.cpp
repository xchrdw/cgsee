#include "abstractStereoCamera.h"

#include <glm/gtc/matrix_access.hpp>
#include <QString>
#include "framebufferobject.h"
#include "camera.h"

AbstractStereoCamera::AbstractStereoCamera(Camera & abstraction)
    :  CameraImplementation(abstraction),
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
    
    m_up = m_abstraction.getUp();    
    m_virtualCameraPosition = m_abstraction.getEye();    
    m_center = m_abstraction.getCenter();    
}
//void AbstractStereoCamera::bindSampler(
//
//    const t_samplerByName & sampler
//, const Program & program)
//{
//    t_samplerByName::const_iterator i(sampler.cbegin());
//    const t_samplerByName::const_iterator iEnd(sampler.cend());
//
//    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
//        i.value()->bindTexture2D(program, i.key(), slot);
//}
//
//void AbstractStereoCamera::releaseSampler(
//    const t_samplerByName & sampler)
//{
//    t_samplerByName::const_iterator i(sampler.cbegin());
//    const t_samplerByName::const_iterator iEnd(sampler.cend());
//
//    for(; i != iEnd; ++i)
//        i.value()->releaseTexture2D();
//
//}