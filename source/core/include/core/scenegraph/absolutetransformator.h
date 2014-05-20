#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>


class Node;

class CORE_API AbsoluteTransformator
{
public:
    AbsoluteTransformator();
    ~AbsoluteTransformator();

    Node *toAbsoluteIgnoreRootTransform(Node *root);

protected:
    Node *toAbsolute(Node *relativeRoot, glm::mat4 transform);
    
    glm::mat4 m_transform;
};
