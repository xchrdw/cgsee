
#include "node.h"


Node::Node(const QString & name)
: m_switch( true )
, m_rf(RF_Relative)
, m_transform(glm::mat4(1))
{
}

Node::~Node()
{
}

void Node::invalidateBoundingBox()
{
    m_aabb.invalidate();

//     t_parents::const_iterator i(m_parents.begin());
//     const t_parents::const_iterator iEnd(m_parents.end());

    if (parent() != nullptr)
        dynamic_cast<Node *>(parent())->invalidateBoundingBox();
//     for(; i != iEnd; ++i)
//         (*i)->invalidateBoundingBox();
}

const glm::mat4 & Node::transform() const
{
    return m_transform;
}

void Node::setTransform(const glm::mat4 & transform)
{
    if(transform == m_transform)
        return;

    m_transform = transform;
    invalidateBoundingBox();
}

const Node::e_ReferenceFrame Node::referenceFrame() const
{
    return m_rf;
}

void Node::setReferenceFrame(const e_ReferenceFrame referenceFrame)
{
    if(referenceFrame == m_rf)
        return;

    m_rf = referenceFrame;
    invalidateBoundingBox();
}

Group * Node::asGroup()
{
    return nullptr;
}