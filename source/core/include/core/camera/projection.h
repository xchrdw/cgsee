#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>


class CORE_API Projection
{
public:
    enum e_Type {ORTHOGONAL, PERSPECTIVE};

    Projection(e_Type type);
    Projection(const Projection & projection);
    virtual ~Projection();

    virtual const glm::ivec2 & viewport() const;
    virtual void setViewport(const glm::ivec2 & size);
    virtual void setViewport(const int width, const int height);

    virtual const glm::mat4 & projection() const;

    virtual const float fovy() const;
    virtual void setFovy(const float fovy);

    virtual const float zNear() const;
    virtual void setZNear(const float z);

    virtual const float zFar() const;
    virtual void setZFar(const float z);

    virtual const float aspect() const;

protected:
    virtual void invalidate() const;
    virtual void update() const;

protected:
    e_Type m_type;

    glm::ivec2 m_viewport;

    mutable bool m_invalid;
    mutable glm::mat4 m_projection;

    float m_fovy;
    float m_zNear;
    float m_zFar;
};
