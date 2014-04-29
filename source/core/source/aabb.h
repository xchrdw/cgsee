﻿
#pragma once

#include <array>
#include <glm/glm.hpp>

#include "declspec.h"


class CGSEE_API AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(glm::vec3 llf, glm::vec3 urb);
    virtual ~AxisAlignedBoundingBox();
    
    const bool extend(const glm::vec3 & vertex);
    const bool extend(const AxisAlignedBoundingBox & aabb);

    const glm::vec3 & center() const;
    const glm::float_t radius() const;

    const glm::vec3 & llf() const;
    const glm::vec3 & urb() const;
    const std::array<glm::vec3, 8> allVertices() const;

    const bool inside(const glm::vec3 & vertex) const;
    const bool outside(const glm::vec3 & vertex) const;

    const void invalidate();
    const bool valid() const;

    const AxisAlignedBoundingBox operator*(const glm::mat4 & rhs) const;
    AxisAlignedBoundingBox & operator*=(const glm::mat4 & rhs);

protected:
    glm::vec3 m_llf;
    glm::vec3 m_urb;

    glm::vec3 m_center;
    glm::float_t m_radius;

    bool m_invalidated;
};
