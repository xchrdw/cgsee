#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <core/scenegraph/defaultdrawmethod.h>

class PolygonalDrawable;

namespace globjects{
    class Program;
}

class CORE_API HighlightingDrawMethod : public DefaultDrawMethod
{
public:
    HighlightingDrawMethod();
    virtual ~HighlightingDrawMethod();

    virtual void draw(globjects::Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
};
