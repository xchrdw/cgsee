#pragma once

#include <core/core_api.h>

#include <array>

#include <glm/glm.hpp>

#include <core/plane.h>

class Camera;
class AbstractCamera;
class AxisAlignedBoundingBox;

class CORE_API ViewFrustum
{
public:
    ViewFrustum(Camera *camera);
    ViewFrustum(AbstractCamera *camera);
    virtual ~ViewFrustum();

    enum e_insideFrustum
    {
        INSIDE_FRUSTUM,
        OUTSIDE_FRUSTUM,
        INTERSECTS_FRUSTUM
    };

    e_insideFrustum contains(const AxisAlignedBoundingBox &aabb, glm::mat4 transform) const;
    bool contains(glm::vec3 point) const;
    void update();

    bool isDirty() const;
    void dirty();

protected:

    Camera *m_camera;
    std::array<Plane, 6> m_planes;
    std::array<glm::vec3, 8> m_vertices;
    bool m_dirty;
};
