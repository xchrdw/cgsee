#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/camera/abstractcamera.h>

#include <core/camera/projection.h>

class Program;
class ViewFrustum;
class Projection;

class CORE_API MonoCameraNew : public AbstractCamera
{
public:

    MonoCameraNew(const QString & name, Projection * projection);
    virtual ~MonoCameraNew();

    virtual bool isStereo() override;

    virtual void setView(const glm::mat4 & view) override;

    virtual const glm::mat4 & view() const;

    virtual ViewFrustum * viewFrustum();
    virtual glm::vec3 eye() const;
    virtual glm::vec3 up() const;
    virtual glm::vec3 center() const;

    virtual void setUniformsIn(const Program & program);

protected:
    virtual void update() override;

protected:
    Projection * m_projection;

    glm::mat4 m_view;

    ViewFrustum *m_viewFrustum;
};
