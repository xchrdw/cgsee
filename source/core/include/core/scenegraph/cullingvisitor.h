#pragma once

#include <core/core_api.h>

#include <vector>
#include <memory>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

#include <core/scenegraph/scenevisitorinterface.h>


class AbstractCamera;
class FrameBufferObject;
class Node;
class ViewFrustum;

namespace glo{
    class Program;
}

class CORE_API CullingVisitor final : public SceneVisitorInterface
{
public:
    typedef std::vector<std::shared_ptr<Node> >::iterator t_nodeIter;
    
    CullingVisitor(AbstractCamera *camera, glo::Program *program, glm::mat4 transform);
    ~CullingVisitor();
    bool operator() (Node &node);

private:
    AbstractCamera *m_camera;
    glo::Program *m_program;
    glm::mat4 m_transform;
    const ViewFrustum *m_viewFrustum;
    bool m_cull;
};
