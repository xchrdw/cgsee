#pragma once

#include <glm/glm.hpp>

#include "core/declspec.h"

class Node;

class CGSEE_API AbsoluteTransformator
{
public:
    AbsoluteTransformator();
    ~AbsoluteTransformator();

    Node *toAbsoluteIgnoreRootTransform(Node *root);

protected:
    Node *toAbsolute(Node *relativeRoot, glm::mat4 transform);
    
    glm::mat4 m_transform;
};
