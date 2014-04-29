#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <core/declspec.h>

class PolygonalDrawable;
class Program;

class CGSEE_API DrawMethod
{
public:
    DrawMethod() {}
    virtual ~DrawMethod() {}

    virtual void draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const =0;
};
