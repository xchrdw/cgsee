#pragma once

#include <GL/glew.h>
#include <QString>

#include <memory>

#include <core/declspec.h>
#include <core/common.h>
#include <core/datacore/vertexindexlist.h>  //includes vertexlist.h


class DataBlockRegistry;
class AxisAlignedBoundingBox;

class CGSEE_API PolygonalGeometry
{
public:
    // TODO: Nur voruebergehend mit default value. Spaeter wieder als ref.
    PolygonalGeometry( DataBlockRegistry * registry = nullptr);
    virtual ~PolygonalGeometry();

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
    // TODO:
//     DataBlockRegistry & m_registry;
    std::shared_ptr<DataBlockRegistry> m_registry;
    t_VertexListP m_datablock;
    QString m_vertListHandle;
    QString m_indicesHandle;
    //t_vec3s m_vertices;
    //t_vec3s m_normals;
    //t_vec2s m_texcs;
    //t_uints m_indices;


};
/*
class CGSEE_API PolygonalGeometry : public Node
{
public:
    PolygonalGeometry(const QString & name = "unnamed");
    virtual ~PolygonalGeometry();

    const GLenum mode() const;
    void setMode(const GLenum mode);

    t_vec3s vertices() const;
    //t_vec3s & vertices();  
    void setVertex(int i, glm::vec3 const& data);
    t_vec3s normals() const;
    //t_vec3s & normals();
    void setNormal(int i, glm::vec3 const& data);
    t_vec2s texcs() const;
    //t_vec2s & texcs();
    void setTexC(int i, glm::vec2 const& data);
    t_uints indices() const;
    //t_uints & indices();
    void setIndex(int i, unsigned int data);

    void resize(unsigned int size);

    virtual void draw(
        const Program & program
    ,   const glm::mat4 & transform);

    virtual const AxisAlignedBoundingBox boundingBox() const;

    // Geometry Computation

    // TODO:
    // * generate triangle strip
    // * vertex cache optimization
    // * per face, per vertex normals

    void retrieveNormals();

protected:
    DataBlockRegistry m_registry;
    QString m_vertListName, m_indicesName;
    //t_vec3s m_vertices;
    //t_vec3s m_normals;
    //t_vec2s m_texcs;

    //t_uints m_indices;

    GLenum  m_mode;
};*/