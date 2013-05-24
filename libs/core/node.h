#pragma once

#include <QObject>
#include <QString>
#include <QSet>
#include <QList>

#include "declspec.h"

#include "aabb.h"


class Program;
class Group;

class CGSEE_API Node : public QObject
{
public:
    enum e_ReferenceFrame
    {
        RF_Relative
    ,   RF_Absolute
    };

public:
    Node( const QString & name );
    virtual ~Node();

    virtual void draw( const Program & program, const glm::mat4 & transform ) = 0;
    
    void switchOn() { m_switch = true; }
    void switchOff() { m_switch = false; }
    inline bool isOn() { return m_switch; }
    
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
    bool m_switch;
    e_ReferenceFrame m_rf;
    glm::mat4 m_transform;

    mutable AxisAlignedBoundingBox m_aabb;
};

