#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>


class CGSEE_API AbsoluteTransformator
{
public:
    AbsoluteTransformator(std::shared_ptr<DataBlockRegistry> registry);
    ~AbsoluteTransformator();

    Node *toAbsolute(Node *root);

protected:
    Node *toAbsolute(Node *relativeRoot, glm::mat4 transform);

private:
    std::shared_ptr<DataBlockRegistry> m_registry;

};
