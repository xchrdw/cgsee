#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QString>

#include <core/scenegraph/drawmethod.h>

class PolygonalDrawable;
class Program;

class CORE_API DefaultDrawMethod : public DrawMethod
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
