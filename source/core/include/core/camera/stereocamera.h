#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/camera/abstractcamera.h>


class MonoCameraNew;
class Program;
class ViewFrustum;

class CORE_API StereoCamera : public AbstractCamera
{
public:

    StereoCamera(const QString & name, Projection * projection);
    virtual ~StereoCamera();

    virtual bool isStereo() override;

    virtual void setView(const glm::mat4 & view) override;

    virtual const MonoCameraNew * leftCamera() const;
    virtual const MonoCameraNew * rightCamera() const;

protected:
    virtual void update() override;

protected:
    Projection * m_projection;
    MonoCameraNew * m_leftCamera;
    MonoCameraNew * m_rightCamera;
};
