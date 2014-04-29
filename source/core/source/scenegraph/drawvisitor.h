#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <core/declspec.h>

#include "scenevisitorinterface.h"


class Program;
class Node;

class CGSEE_API DrawVisitor : public SceneVisitorInterface
{
public:
    DrawVisitor(const Program * p, glm::mat4 t);
    virtual bool operator() (Node & node) override;

private:
    const Program * m_program;
    glm::mat4 m_transform;
};
