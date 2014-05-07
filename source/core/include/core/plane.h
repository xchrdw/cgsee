#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>


class CORE_API Plane 
{
public:
    Plane();
    Plane(glm::vec4 normal, float distance);
    Plane(glm::vec4 normal, glm::vec4 supportVector);
    Plane(glm::vec4 normal, float distance, glm::mat4 transform);
    virtual ~Plane();

    Plane transformed(glm::mat4 transform);
    
    glm::vec4 normal();
    float distance();

private:
    glm::vec4 m_normal; // is normalized
    float m_distance; // distance from origin
};
