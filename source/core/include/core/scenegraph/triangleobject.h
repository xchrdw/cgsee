#pragma once

#include <core/core_api.h>

#include <vector>

#include <glm/glm.hpp>

#include <core/scenegraph/node.h>

namespace glo{
    class Program;
}

typedef std::vector<glm::vec3> *p_TriangleList;

class CORE_API TriangleObject : public Node
{
public:
    TriangleObject(const QString & name = "unnamed");
    virtual ~TriangleObject();

    virtual void draw(glo::Program & program, const glm::mat4 & transform) override;

    virtual const AxisAlignedBoundingBox boundingBox() const override;
    virtual const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const override;

    virtual p_TriangleList triangles();

protected:
    virtual void invalidateBoundingBox() override;
    p_TriangleList m_triangles;
};
