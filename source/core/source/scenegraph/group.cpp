
#include <core/scenegraph/group.h>

#include <core/program.h>

#include <core/scenegraph/polygonaldrawable.h>
#include <core/aabb.h>


Group::Group(const QString & name)
:   Node(name),
    m_invalidatedChildren(true)
{
}

Group::~Group()
{
    while(!m_children.empty())
        removeLast();
}

void Group::draw(const Program & program, const glm::mat4 & transform)
{
}

const bool Group::contains(Node * node) const
{
    return std::find(m_children.begin(), m_children.end(), node) != m_children.end();
}

void Group::insert(const Group::t_children::iterator & before, Group * group)
{
    return insert(before, dynamic_cast<Node *>(group));
}

void Group::insert(const Group::t_children::iterator & before, PolygonalDrawable * drawable)
{
    return insert(before, dynamic_cast<Node *>(drawable));
}

void Group::insert(const Group::t_children::iterator & before, Node * node)
{
    if(!node)
        return;

    invalidateChildren();

    if(!contains(node))
        node->parents().push_back(this);

    m_children.insert(before, node);
}

void Group::prepend(Group * group)
{
    return prepend(dynamic_cast<Node *>(group));
}

void Group::prepend(PolygonalDrawable * drawable)
{
    return prepend(dynamic_cast<Node *>(drawable));
}

void Group::prepend(Node * node)
{
    if(!node)
        return;
    
    invalidateChildren();

    if(!contains(node))
        node->parents().push_back(node);

    m_children.push_front(node);
}

void Group::append(Group * group)
{
    return append(dynamic_cast<Node *>(group));
}

void Group::append(PolygonalDrawable * drawable)
{
    return append(dynamic_cast<Node *>(drawable));
}

void Group::append(Node * node)
{
    if(!node)
        return;

    invalidateChildren();

    if(!contains(node))
        node->parents().push_back(this);

    m_children.push_back(node);
}

void Group::removeFirst()
{
    if(m_children.empty())
        return;

    invalidateChildren();

    Node * node(m_children.front());

    node->parents().remove(this);
    m_children.pop_front();

    if(node->parents().empty())
        delete node;
}

void Group::removeLast()
{
    if(m_children.empty())
        return;
    
    invalidateChildren();

    Node * node(m_children.back());

    node->parents().remove(this);
    m_children.pop_back();

    if(node->parents().empty())
        delete node;
}

const void Group::remove(Node * node, const bool deleteIfParentsEmpty)
{
    invalidateChildren();

    if(!contains(node))
        node->parents().remove(this);

    m_children.remove(node);

    if(deleteIfParentsEmpty && node->parents().empty())
        delete node;
}

const AxisAlignedBoundingBox Group::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    t_children::const_iterator i(m_children.begin());
    const t_children::const_iterator iEnd(m_children.end());

    if(RF_Relative == m_rf)
        for(; i != iEnd; ++i)
            m_aabb.extend((*i)->boundingBox() * transform());
    else
        for(; i != iEnd; ++i)
            m_aabb.extend((*i)->boundingBox());

    return m_aabb;
}

const AxisAlignedBoundingBox Group::boundingBox(glm::mat4 transform) const
{
    AxisAlignedBoundingBox aabb = AxisAlignedBoundingBox();
    t_children::const_iterator i(m_children.begin());
    const t_children::const_iterator iEnd(m_children.end());
    glm::mat4 newTransform;

    if (RF_Relative == m_rf) {
        newTransform = this->transform() * transform;
    } else {
        newTransform = transform;
    }

    for(; i != iEnd; ++i) {
        aabb.extend((*i)->boundingBox(newTransform));
    }

    return aabb;
}

Group * Group::asGroup()
{
    return this;
}

void Group::invalidateChildren()
{
    if (m_invalidatedChildren)
        return;

    m_invalidatedChildren = true;
}
