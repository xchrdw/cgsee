#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/scenegraph/group.h>

class Program;
class ViewFrustum;

class CORE_API AbstractCamera : public Group
{
public:

    AbstractCamera(const QString & name = "unnamed");
    virtual ~AbstractCamera();

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & size);
    void setViewport(const int width, const int height);

    const glm::mat4 & projection();

    const glm::mat4 & view() const;
    void setView(const glm::mat4 & view);

    const float fovy() const;
    void setFovy(const float fovy);

    const float zNear() const;
    void setZNear(const float z);

    const float zFar() const;
    void setZFar(const float z);

    const float aspect() const;

    ViewFrustum * viewFrustum();

    // updates camera matrices
    void update();
    void setUniforms(const Program & program);

    glm::vec3 getEye() const;
    glm::vec3 getUp() const;
    glm::vec3 getCenter() const;

protected:
    void invalidate();
    virtual void invalidateChildren() override;

    glm::ivec2 m_viewport;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    ViewFrustum *m_viewFrustum;

    float m_fovy;
    float m_zNear;
    float m_zFar;

    bool m_invalidated;
};
