#include <GL/glew.h>

#include "polygonaldrawable.h"

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "polygonalgeometry.h"
#include "defaultdrawmethod.h"

// TODO: wieder rueckgaengig machen...
// PolygonalDrawable::PolygonalDrawable( DataBlockRegistry & registry, const QString & name )
PolygonalDrawable::PolygonalDrawable(const QString & name)
:   Node( name )
,   m_geometry( nullptr )
,   m_mode( GL_TRIANGLES )
,   m_drawMehtod( new DefaultDrawMethod )
{
}

PolygonalDrawable::~PolygonalDrawable()
{
    delete m_drawMehtod;
}

void PolygonalDrawable::setGeometry(t_geometryP geometry)
{
    m_geometry = geometry;
}

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox() const
{
    if( m_aabb.valid() )
        return m_aabb;

    if( m_geometry == nullptr )
        return AxisAlignedBoundingBox();

    t_VertexListP myVList = m_geometry->vertices();
    AxisAlignedBoundingBox & aabb = m_aabb;
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&aabb](int i, const glm::vec3 & pos)
        {
            aabb.extend( pos );
        }
    );

    return m_aabb;
}

void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::draw(const Program & program, const glm::mat4 & transform)
{
    if (m_geometry)
    {
        m_drawMehtod->draw(program, transform, *this);
    }
}

void PolygonalDrawable::setDrawMethod( DrawMethod * drawMethod )
{
    delete m_drawMehtod;
    m_drawMehtod = drawMethod;
}

DrawMethod * PolygonalDrawable::drawmethod(void)
{
    return m_drawMehtod;
}
