#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <core/declspec.h>

#include "drawmethod.h"

class PolygonalDrawable;
class Program;

class CGSEE_API DefaultDrawMethod : public DrawMethod
{
public:
    DefaultDrawMethod();
    virtual ~DefaultDrawMethod();

    virtual void draw(const Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
    
protected:
    glm::vec4 colorVectorFromId(const unsigned int id) const;

    const QString TRANSFORM_UNIFORM = "transform";
    const QString ID_UNIFORM = "u_id";
    const QString MATERIAL_UNIFORM = "material";    
};
