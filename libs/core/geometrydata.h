#pragma once

#include <GL/glew.h>

#include <QString>

#include "declspec.h"

#include "common.h"
#include "datacore/vertexlist.h"

class AxisAlignedBoundingBox;

class CGSEE_API GeometryData
{
public:
    GeometryData( DataBlockRegistry & registry );
    virtual ~GeometryData();

    t_vec3s vertices() const;
    //t_vec3s & vertices();  
    void setVertex( int i, glm::vec3 const & data );
    t_vec3s normals() const;
    //t_vec3s & normals();
    void setNormal( int i, glm::vec3 const & data );
    t_vec2s texcs() const;
    //t_vec2s & texcs();
    void setTexC( int i, glm::vec2 const & data );
    t_uints indices() const;
    //t_uints & indices();
    void setIndex( int i, unsigned int data );

    void resize( unsigned int size );

    // Geometry Computation

    // TODO:
    // * generate triangle strip
    // * vertex cache optimization
    // * per face, per vertex normals

    // TODO: Nach PolygonalDrawable?
    void retrieveNormals();

protected:
    DataBlockRegistry & m_registry;
    QString m_vertListHandle;
    QString m_indicesHandle;
    //t_vec3s m_vertices;
    //t_vec3s m_normals;
    //t_vec2s m_texcs;
    //t_uints m_indices;


};