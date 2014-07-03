#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>
#include <QString>

#include <core/scenegraph/group.h>

#include <core/viewfrustum.h>

class Projection;

class CORE_API AbstractCamera : public Group
{
public:

    static const QString VIEWPORT_UNIFORM;
    static const QString VIEW_UNIFORM;
    static const QString PROJECTION_UNIFORM;
    static const QString TRANSFORM_UNIFORM;
    static const QString TRANSFORMINVERSE_UNIFORM;
    static const QString ZNEAR_UNIFORM;
    static const QString ZFAR_UNIFORM;
    static const QString CAMERAPOSITION_UNIFORM;

    AbstractCamera(const QString & name = "unnamed");
    virtual ~AbstractCamera();

    virtual bool isStereo() = 0;

    virtual void setProjection(Projection * projection);
    virtual const Projection * getProjection() const;

    virtual void setViewport(const glm::ivec2 & size);
    virtual void setViewport(const int width, const int height);
    virtual const glm::ivec2 & viewport() const;
    virtual const float aspect() const;

    virtual const float fovy() const;
    virtual void setFovy(const float fovy);
    virtual const float zNear() const;
    virtual void setZNear(const float z);
    virtual const float zFar() const;
    virtual void setZFar(const float z);
    virtual const glm::mat4 & projection() const;

    virtual void setView(const glm::mat4 & view) = 0;

    virtual void invalidate();

protected:
    virtual void update();
    virtual void invalidateChildren() override;

protected:
    mutable bool m_invalid;
    Projection * m_projection;
};