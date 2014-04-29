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

    static const QString TRANSFORM_UNIFORM;
    static const QString ID_UNIFORM;
    static const QString MATERIAL_UNIFORM;
};
