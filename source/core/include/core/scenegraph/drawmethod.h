#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>


class PolygonalDrawable;
class Program;

class CORE_API DrawMethod
{
public:
    DrawMethod() {}
    virtual ~DrawMethod() {}

    virtual void draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const =0;
};
