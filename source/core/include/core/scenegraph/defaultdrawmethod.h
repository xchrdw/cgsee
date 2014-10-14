#pragma once

#include <core/core_api.h>

#include <string>

#include <glm/glm.hpp>

#include <core/scenegraph/drawmethod.h>

class PolygonalDrawable;

namespace globjects{
    class Program;
}

class CORE_API DefaultDrawMethod : public DrawMethod
{
public:
    DefaultDrawMethod();
    virtual ~DefaultDrawMethod();

    virtual void draw(globjects::Program & program, const glm::mat4 & transform, PolygonalDrawable & drawable) const override;
    
protected:
    glm::vec4 colorVectorFromId(const unsigned int id) const;

    static const std::string TRANSFORM_UNIFORM;
    static const std::string ID_UNIFORM;
    static const std::string MATERIAL_UNIFORM;
};
