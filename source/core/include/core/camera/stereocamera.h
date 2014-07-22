#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/camera/abstractcamera.h>


class MonoCamera;
class Program;
class ViewFrustum;

class CORE_API StereoCamera : public AbstractCamera
{
public:

    StereoCamera(const QString & name, Projection * projection);
    virtual ~StereoCamera();

    virtual bool isStereo() override;

    virtual const MonoCamera * leftCamera();
    virtual const MonoCamera * rightCamera();

protected:
    virtual void recalculate() override;

protected:
    MonoCamera * m_leftCamera;
    MonoCamera * m_rightCamera;
};
