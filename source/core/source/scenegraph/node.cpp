
#include <core/scenegraph/node.h>


Node::Node(const QString & name)
:   m_name(name)
,   m_parents()
,   m_children()
,   m_rf(RF_Relative)
,   m_transform(glm::mat4(1))
,   m_selected(false)
,   m_hidden(false)
{
}

Node::~Node()
{
}

const QString Node::name() const
{
    return m_name;
}

void Node::setName(const QString & name)
{
    m_name = name;
}

const Node::t_parents & Node::parents() const
{
    return m_parents;
}

Node::t_parents & Node::parents()
{
    return m_parents;
}

const Node::t_children & Node::children() const
{
    return m_children;
}

void Node::invalidateBoundingBox()
{
    m_aabb.invalidate();

    t_parents::const_iterator i(m_parents.begin());
    const t_parents::const_iterator iEnd(m_parents.end());

    for(; i != iEnd; ++i)
        (*i)->invalidateBoundingBox();
}

const glm::mat4 & Node::transform() const
{
    return m_transform;
}

const glm::mat4 & Node::transformInverse() const
{
    return m_transformInverse;
}

void Node::setTransform(const glm::mat4 & transform)
{
    if(transform == m_transform)
        return;

    m_transform = transform;
    m_transformInverse = glm::inverse(transform);
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

const unsigned int Node::id() const
{
    return m_id;
}

void Node::setId(const unsigned int id)
{
    m_id = id;
}

const bool Node::selected() const
{
    return m_selected;
}

void Node::setSelected(const bool selected)
{
    m_selected = selected;
}

const bool Node::hidden() const
{
    return m_hidden;
}

void Node::setHidden(const bool hidden)
{
    m_hidden = hidden;
}
