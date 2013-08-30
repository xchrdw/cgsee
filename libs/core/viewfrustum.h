#pragma once

#include <array>

#include "declspec.h"
#include "glm/glm.hpp"
#include "core/plane.h"

class Camera;
class AxisAlignedBoundingBox;

class CGSEE_API ViewFrustum
{
public:
    ViewFrustum(Camera *camera);
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
