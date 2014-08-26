
#include <core/scenegraph/cullingvisitor.h>

#include <vector>
#include <memory>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

#include <globjects/Program.h>

#include <core/camera/abstractcamera.h>
#include <core/scenegraph/node.h>
#include <core/viewfrustum.h>


CullingVisitor::CullingVisitor(AbstractCamera *camera, glo::Program *program, glm::mat4 transform) :
    m_camera(camera),
    m_program(program),
    m_transform(transform),
    m_viewFrustum(camera->viewFrustum()),
    m_cull(true)
{
}

CullingVisitor::~CullingVisitor()
{
}

bool CullingVisitor::operator() (Node &node) {
    bool returnValue, drawNode;
    if (m_cull) {
        switch (m_viewFrustum->contains(node.boundingBox(), m_transform)) {
        case ViewFrustum::e_insideFrustum::INSIDE_FRUSTUM:
            m_cull = false;
            returnValue = drawNode = true;
        case ViewFrustum::e_insideFrustum::OUTSIDE_FRUSTUM:
            //YAY - culled!
            returnValue = drawNode = false;
        default: //ViewFrustum::e_insideFrustum::INTERSECTS_FRUSTUM:
            m_cull = true;
            returnValue = drawNode = true;
        }
    }
    if (drawNode) {
        node.draw(*m_program, m_transform);
    }

    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return returnValue;
}
