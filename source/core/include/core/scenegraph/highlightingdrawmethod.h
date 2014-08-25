#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <core/scenegraph/defaultdrawmethod.h>

class PolygonalDrawable;

namespace glo{
    class Program;
}

class CORE_API HighlightingDrawMethod : public DefaultDrawMethod
{
public:
    HighlightingDrawMethod();
    virtual ~HighlightingDrawMethod();

    virtual void draw(glo::Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
};
