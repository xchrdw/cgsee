#pragma once

#include <core/core_api.h>

#include <glm/gtc/matrix_transform.hpp>

#include <core/scenegraph/scenevisitorinterface.h>

class Node;

namespace globjects{
    class Program;
}

class CORE_API DrawVisitor : public SceneVisitorInterface
{
public:
    DrawVisitor(globjects::Program * p, glm::mat4 t);
    virtual bool operator() (Node & node) override;

private:
    globjects::Program * m_program;
    glm::mat4 m_transform;
};
