#pragma once

#include <core/core_api.h>

#include <vector>
#include <memory>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

#include <core/scenegraph/scenevisitorinterface.h>


class Camera;
class FrameBufferObject;
class Node;
class ViewFrustum;
class Program;

class CORE_API CullingVisitor final : public SceneVisitorInterface
{
public:    
    CullingVisitor(Camera *camera, Program *program, glm::mat4 transform);
    ~CullingVisitor();
    bool operator() (Node &node) override;

private:
    Camera *m_camera;
    Program *m_program;
    glm::mat4 m_transform;
    ViewFrustum *m_viewFrustum;
    bool m_cull;
};
