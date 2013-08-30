#include "glm/glm.hpp"

#include "plane.h"

Plane::Plane() :
    m_distance(1.0),
    m_normal(glm::vec3(1.0, 0.0, 0.0))
{
}

Plane::Plane(glm::vec3 normal, float distance) :
    m_distance(distance),
    m_normal(glm::normalize(normal))
{
}

Plane::Plane(glm::vec3 normal, glm::vec3 supportVector) :
    m_distance(0.0f),
    m_normal(glm::normalize(normal))
{
    m_distance = glm::dot(supportVector, m_normal);
}

Plane::Plane(glm::vec3 normal, float distance, glm::mat4 transform) :
    m_distance(distance), // value of m_distance is not yet valid
    m_normal(normal) // value of m_normal is not yet valid
{
    glm::vec4 homogenousNormal = glm::normalize(transform * glm::vec4(normal, 1.0));
    m_normal = (1 / homogenousNormal.w) * homogenousNormal.xyz;
    
    glm::vec4 homogenousSupportVector = transform * (homogenousNormal * distance);
    glm::vec3 supportVector = (1 / homogenousSupportVector.w) * homogenousSupportVector.xyz;

    m_distance = glm::dot(supportVector, m_normal);
}

    
Plane::~Plane() {
}

glm::vec3 Plane::normal() {
    return m_normal;
}

float Plane::distance() {
    return m_distance;
}
