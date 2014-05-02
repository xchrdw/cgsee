#pragma once

#include <core/datacore/vertexindexlist.h>

class GeometryOptimizer 
{
public:
    virtual void applyOn(t_VertexIndexListP vertexIndices, t_VertexListP vertexData) = 0;
};
