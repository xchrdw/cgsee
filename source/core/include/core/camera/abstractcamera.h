#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>
#include <QString>

#include <core/scenegraph/group.h>

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

    virtual const glm::ivec2 & viewport() const = 0;
    virtual void setViewport(const glm::ivec2 & size) = 0;
    virtual void setViewport(const int width, const int height) = 0;

    virtual const glm::mat4 & projection() = 0;

    virtual const glm::mat4 & view() const = 0;
    virtual void setView(const glm::mat4 & view) = 0;

    virtual const float fovy() const = 0;
    virtual void setFovy(const float fovy) = 0;

    virtual const float zNear() const = 0;
    virtual void setZNear(const float z) = 0;

    virtual const float zFar() const = 0;
    virtual void setZFar(const float z) = 0;

    virtual const float aspect() const = 0;

    virtual ViewFrustum * viewFrustum() = 0;

    // updates camera matrices
    virtual void update() = 0;
    virtual void setUniforms(const Program & program) = 0;

    virtual glm::vec3 getEye() const = 0;
    virtual glm::vec3 getUp() const = 0;
    virtual glm::vec3 getCenter() const = 0;

protected:
    void invalidate();
    virtual void invalidateChildren() override;

    bool m_invalidated;
};
