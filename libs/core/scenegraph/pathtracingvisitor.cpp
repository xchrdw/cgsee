#include "pathtracingvisitor.h"
#include "polygonalgeometry.h"
#include "polygonaldrawable.h"
#include "../bufferobject.h"
#include "../pathtracer.h"
#include "../program.h"

PathTracingVisitor::PathTracingVisitor(Program & p, PathTracer & pathTracer)
    : m_program(p)
    , m_pathTracer(pathTracer)
{}

bool PathTracingVisitor::operator() (Node & node)
{
    PolygonalDrawable * drawable = dynamic_cast<PolygonalDrawable*>(&node);
    if (drawable == nullptr)
        return true;

    PolygonalDrawable::t_geometryP geometry = drawable->geometry();
    // initialize the geometry to have it's data linked to the gpu
    geometry->initialize(m_program);

    initPathTracingTextures(*geometry);

    // ... push the geometry to m_pathTracer or something, to create the hierarchical geometry

    // stop here, as we only support one object at the moment
    return false;
}

void PathTracingVisitor::initPathTracingTextures(PolygonalGeometry & geometry)
{
    // get buffer names from geometry
    // only one list of indices is supported at the moment
    GLuint indexBuffer = geometry.elementArrayBOs().first()->buffer();
    GLuint vertexBuffer = geometry.arrayBOsByAttribute()["a_vertex"]->buffer();
    GLuint normalBuffer = geometry.arrayBOsByAttribute()["a_normal"]->buffer();

    GLuint indexTextureID, vertexTextureID, normalTextureID;

    glActiveTexture(GL_TEXTURE0 + PathTracer::textureSlots["indexBuffer"]);
    glGenTextures(1, &indexTextureID);
    glBindTexture(GL_TEXTURE_BUFFER, indexTextureID);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, indexBuffer);
    //glBindTexture(GL_TEXTURE_BUFFER, 0);
    glError();

    glActiveTexture(GL_TEXTURE0+PathTracer::textureSlots["vertexBuffer"]);
    glGenTextures(1, &vertexTextureID);
    glBindTexture(GL_TEXTURE_BUFFER, vertexTextureID);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, vertexBuffer);
    //glBindTexture(GL_TEXTURE_BUFFER, 0);
    glError();

    // normal data
    glActiveTexture(GL_TEXTURE0+PathTracer::textureSlots["normalBuffer"]);
    glGenTextures(1, &normalTextureID);
    glBindTexture(GL_TEXTURE_BUFFER, normalTextureID);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, normalBuffer);
    //glBindTexture(GL_TEXTURE_BUFFER, 0);
    glError();
}