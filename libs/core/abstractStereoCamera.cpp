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

void AbstractStereoCamera::setFromMatrix(const glm::mat4 & view){ 
    
    //Extract Up Vector and Viewing direction from viewmatrix
    m_up = glm::row(view, 1).xyz;
    
    glm::vec3 lookat = glm::row(view, 2).xyz;
    
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
    // (using math from RealTime Collision Detection by Christer Ericson)
    glm::vec3 n2n3 = glm::cross(n2, n3);
    float denom = glm::dot(n1, n2n3);
    
    m_virtualCameraPosition = (n2n3 * d1) + glm::cross(n1, (d3*n2) - (d2*n3));
    m_virtualCameraPosition /= -denom;
    
    m_center = m_virtualCameraPosition - lookat;
    
}