#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/camera/abstractcamera.h>
#include <core/camera/monocamera.h>

class Program;
class ViewFrustum;

class CORE_API StereoCamera : public AbstractCamera
{
public:

    StereoCamera(const QString & name = "unnamed");
    virtual ~StereoCamera();

    virtual const glm::ivec2 & viewport() const override;
    virtual void setViewport(const glm::ivec2 & size) override;
    virtual void setViewport(const int width, const int height) override;

    virtual const glm::mat4 & projection() override;

    virtual const glm::mat4 & view() const override;
    virtual void setView(const glm::mat4 & view) override;

    virtual const float fovy() const override;
    virtual void setFovy(const float fovy) override;

    virtual const float zNear() const override;
    virtual void setZNear(const float z) override;

    virtual const float zFar() const override;
    virtual void setZFar(const float z) override;

    virtual const float aspect() const override;

    virtual ViewFrustum * viewFrustum() override;

    // updates camera matrices
    virtual void update() override;
    virtual void setUniforms(const Program & program) override;

    virtual glm::vec3 getEye() const override;
    virtual glm::vec3 getUp() const override;
    virtual glm::vec3 getCenter() const override;

protected:
    MonoCamera & m_leftCamera;
    MonoCamera & m_rightCamera;
};