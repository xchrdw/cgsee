#pragma once

#include <core/core_api.h>

#include <GL/glew.h>

#include <memory>

#include <glm/glm.hpp>

#include <QMap>
#include <QVector>

#include <core/scenegraph/node.h>


class PolygonalGeometry;
class DataBlockRegistry;
class Program;
class DrawMethod;


class CORE_API PolygonalDrawable : public Node
{
public:
    typedef std::shared_ptr<PolygonalGeometry> t_geometryP;
    typedef std::shared_ptr<DrawMethod> t_drawMethodP;
    
    // TODO: wieder rueckgaengig machen...
//     PolygonalDrawable( DataBlockRegistry & registry, const QString & name );
    PolygonalDrawable(const QString & name = "unnamed");
    virtual ~PolygonalDrawable();

    virtual void draw(const Program & program, const glm::mat4 & transform) override;

    virtual const AxisAlignedBoundingBox boundingBox() const override;
    virtual const AxisAlignedBoundingBox boundingBox(glm::mat4 transform) const override;

    void setGeometry(t_geometryP geometry);
    t_geometryP geometry() { return m_geometry; }

    void setMode(const GLenum mode) { m_mode = mode; }
    inline const GLenum mode(void) const { return m_mode; }
    
    void setDrawMethod( t_drawMethodP drawmethod );
    t_drawMethodP drawmethod(void);

    unsigned int numVertices() const;
    
protected:
    virtual void invalidateBoundingBox() override;

protected:
    t_geometryP m_geometry;
    GLenum  m_mode;
    t_drawMethodP m_drawMethod;
};
