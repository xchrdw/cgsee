#pragma once

#include <glm/glm.hpp>

#include "declspec.h"

class CGSEE_API Plane {
public:
    Plane();
    Plane(glm::vec3 normal, float distance);
    Plane(glm::vec3 normal, glm::vec3 supportVector);
    Plane(glm::vec3 normal, float distance, glm::mat4 transform);
    virtual ~Plane();

    glm::vec3 normal();
    float distance();

private:
    glm::vec3 m_normal; // is normalized
    float m_distance; // distance from origin
};
