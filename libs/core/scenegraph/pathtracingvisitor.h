#pragma once

#include <core/declspec.h>
#include "scenevisitorinterface.h"

class Program;
class PathTracer;
class PolygonalGeometry;

class CGSEE_API PathTracingVisitor : public SceneVisitorInterface
{
public:
    PathTracingVisitor(Program & p, PathTracer & pathTracer);
    virtual bool operator() (Node & node);

protected:
    // Make indices, vertices and normals accessible as indexed texture
    void initPathTracingTextures(PolygonalGeometry & geometry) ;

    Program & m_program;
    PathTracer & m_pathTracer;
};
