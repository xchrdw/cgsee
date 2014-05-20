#pragma once

#include <core/core_api.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <core/scenegraph/defaultdrawmethod.h>

class PolygonalDrawable;
class Program;

class CORE_API HighlightingDrawMethod : public DefaultDrawMethod
{
public:
    HighlightingDrawMethod();
    virtual ~HighlightingDrawMethod();

    virtual void draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
};
