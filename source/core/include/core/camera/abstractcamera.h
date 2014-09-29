#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>


#include <QVector>
#include <QString>

#include <signalzeug/Signal.h>

#include <core/scenegraph/group.h>

class Projection;
class Program;
class ViewFrustum;

class CORE_API AbstractCamera : public Group
{
public:

    AbstractCamera(const QString & name, Projection * projection);
    AbstractCamera(const AbstractCamera & camera);

    virtual ~AbstractCamera();

    virtual bool isStereo() const = 0;

    virtual glm::mat4 viewProjection();

    virtual void setProjection(Projection * projection);
    virtual Projection * getProjection() const;

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

    virtual void setView(const glm::mat4 & view);
    virtual const glm::mat4 & view() const;

    virtual const ViewFrustum * viewFrustum();
    virtual glm::vec3 eye() const;
    virtual glm::vec3 up() const;
    virtual glm::vec3 center() const;

    virtual void invalidate();

    virtual const signalzeug::Signal<> & viewChangedSignal();

protected:
    virtual void update();
    virtual void recalculate();
    virtual void invalidateChildren() override;

protected:
    mutable bool m_invalid;
    signalzeug::Signal<> m_viewChanged;

    Projection * m_projection;

    glm::mat4 m_view;
    ViewFrustum * m_viewFrustum;

    glm::mat4 m_viewProjection;
};