#pragma once

#include <core/core_api.h>

#include <core/scenegraph/node.h>

class PolygonalDrawable;

namespace globjects{
    class Program;
}

class CORE_API Group : public Node
{
public:
    Group(const QString & name = "unnamed");
    virtual ~Group();

    virtual Group * asGroup();

    virtual void draw(globjects::Program & program, const glm::mat4 & transform);

    const bool contains(Node * node) const;

    void prepend(Group * group);
    void prepend(PolygonalDrawable * drawable);

    void append(Group * group);
    void append(PolygonalDrawable * drawable);

    void insert(const t_children::iterator & before, Group * group);
    void insert(const t_children::iterator & before, PolygonalDrawable * drawable);


    // both removals delete the node if parents are empty
    virtual void removeFirst();
    virtual void removeLast ();

    virtual const void remove(Node * node, const bool deleteIfParentsEmpty = true);

    virtual const AxisAlignedBoundingBox boundingBox() const;
    virtual const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const;

    virtual void prepend(Node * node);
    virtual void append(Node * node);

    virtual void insert(const t_children::iterator & before, Node * node);

protected:
    virtual void invalidateChildren();
    bool m_invalidatedChildren;
};
