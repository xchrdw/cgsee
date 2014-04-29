#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <core/declspec.h>

#include "defaultdrawmethod.h"

class PolygonalDrawable;
class Program;

class CGSEE_API HighlightingDrawMethod : public DefaultDrawMethod
{
public:
    HighlightingDrawMethod();
    virtual ~HighlightingDrawMethod();

    virtual void draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
};
