#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <QMap>
#include <QVector>

#include "declspec.h"
#include "node.h"


class GeometryData;
class DataBlockRegistry;
class BufferObject;
class Program;


class CGSEE_API PolygonalDrawable : public Node
{
public:
    PolygonalDrawable( DataBlockRegistry & registry, const QString & name );
    virtual ~PolygonalDrawable();

    void setGeometry( std::shared_ptr<GeometryData> geometry );
    std::shared_ptr<GeometryData> geometry();

    void setMode( const GLenum mode ) { m_mode = mode; }
    inline const GLenum mode() const { return m_mode; }

    virtual const AxisAlignedBoundingBox boundingBox() const override;
    
    virtual void draw( const Program & program, const glm::mat4 & transform ) override;

protected:
    void initialize(const Program & program);
    void deleteBuffers();
    virtual void invalidateBoundingBox() override;

protected:
    GLuint m_vao;

    std::shared_ptr<GeometryData> m_geometry;
    GLenum  m_mode;
    
    typedef QVector<BufferObject *> t_bufferObjects;
    t_bufferObjects m_elementArrayBOs;

    typedef QMap<QString, BufferObject *> t_bufferObjectsByAttribute;
    t_bufferObjectsByAttribute m_arrayBOsByAttribute;    
};