#pragma once

#include <list>

#include <QString>

#include <core/declspec.h>
#include <core/aabb.h>


class Program;

class CGSEE_API Node
{
public:
    typedef std::list<Node *> t_children;
    typedef std::list<Node *> t_parents;

    enum e_ReferenceFrame
    {
        RF_Relative
    ,   RF_Absolute
    };

public:
    Node(const QString & name);
    virtual ~Node();

    virtual void draw(const Program & program, const glm::mat4 & transform) = 0;
    virtual const AxisAlignedBoundingBox boundingBox() const = 0;
    virtual const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const = 0;

    const QString name() const;
    void setName(const QString & name);

    const t_parents & parents() const;
    t_parents & parents();

    const t_children & children() const;
    
    const glm::mat4 & transform() const;
    const glm::mat4 & transformInverse() const;
    void setTransform(const glm::mat4 & transform);

    const e_ReferenceFrame referenceFrame() const;
    void setReferenceFrame(const e_ReferenceFrame referenceFrame);

    const unsigned int id() const;
    void setId(const unsigned int id);

    const bool selected() const;
    void setSelected(const bool selected);

    const bool hidden() const;
    void setHidden(const bool hidden);
    
//     bool isCircularDependentTo( const Node & other ) const;

protected:
    virtual void invalidateBoundingBox();

protected:
    QString m_name;
    t_parents m_parents;
    t_children m_children;
    e_ReferenceFrame m_rf;
    glm::mat4 m_transform;
    glm::mat4 m_transformInverse;
    mutable AxisAlignedBoundingBox m_aabb;

    unsigned int m_id;
    bool m_selected;
    bool m_hidden;
};

