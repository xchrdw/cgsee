#pragma once

#include <QString>
#include <QSet>
#include <QList>

#include "declspec.h"

#include "aabb.h"


class Program;
class Group;

class CGSEE_API Node
{
public:
    typedef QList<Node *> t_nodes;
    typedef QSet<Node *> t_parents;

    enum e_ReferenceFrame
    {
        RF_Relative
    ,   RF_Absolute
    };

public:
    Node(const QString & name = "unnamed");
    virtual ~Node();

    virtual void draw(
        Program * program
    ,   const glm::mat4 & transform) = 0;

    const QString name() const;
    void setName(const QString & name);

    const t_parents & parents() const;
    t_parents & parents();

    virtual const AxisAlignedBoundingBox boundingBox() const = 0;

    // transform

    const glm::mat4 & transform() const;
    void setTransform(const glm::mat4 & transform);

    const e_ReferenceFrame referenceFrame() const;
    void setReferenceFrame(const e_ReferenceFrame referenceFrame);

    // This saves casting during traversal.
    virtual Group * asGroup();

protected:
    virtual void invalidateBoundingBox();

protected:
    QString m_name;

    t_parents m_parents;

    e_ReferenceFrame m_rf;
    glm::mat4 m_transform;

    mutable AxisAlignedBoundingBox m_aabb;
};

