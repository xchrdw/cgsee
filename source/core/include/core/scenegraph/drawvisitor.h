#pragma once

#include <core/core_api.h>

#include <glm/gtc/matrix_transform.hpp>

#include <core/scenegraph/scenevisitorinterface.h>


class Program;
class Node;

class CORE_API DrawVisitor : public SceneVisitorInterface
{
public:
    DrawVisitor(const Program * p, glm::mat4 t);
    virtual bool operator() (Node & node) override;

private:
    const Program * m_program;
    glm::mat4 m_transform;
};
