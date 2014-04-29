#include <GL/glew.h>

#include "polygonaldrawable.h"

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "polygonalgeometry.h"
#include "defaultdrawmethod.h"

static const QString TRANSFORM_UNIFORM( "transform" );

// TODO: wieder rueckgaengig machen...
// PolygonalDrawable::PolygonalDrawable( DataBlockRegistry & registry, const QString & name )
PolygonalDrawable::PolygonalDrawable(const QString & name)
:   Node( name )
,   m_geometry( std::shared_ptr<PolygonalGeometry>())
,   m_mode( GL_TRIANGLES )
,   m_drawMethod( new DefaultDrawMethod )
{
}

PolygonalDrawable::~PolygonalDrawable()
{
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

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox(glm::mat4 transform) const
{
    AxisAlignedBoundingBox aabb = AxisAlignedBoundingBox();
    glm::mat4 newTransform;
    
    if( m_geometry == nullptr ) {
        return aabb;
    }

    if (m_rf == RF_Absolute) {
        newTransform = transform;
    } else {
        newTransform = this->transform() * transform;
    }


    t_VertexListP myVList = m_geometry->vertices();
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&aabb, &newTransform](int i, const glm::vec3 & pos)
        {
            aabb.extend( glm::vec3(newTransform * glm::vec4(pos, 1.0f)) );
        }
    );

    return aabb;
}

void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::draw(const Program & program, const glm::mat4 & transform)
{
    if (m_geometry)
    {
        m_drawMethod->draw(program, transform, *this);
    }
}

void PolygonalDrawable::setDrawMethod( t_drawMethodP drawMethod )
{
    m_drawMethod = drawMethod;
}

PolygonalDrawable::t_drawMethodP PolygonalDrawable::drawmethod(void)
{
    return m_drawMethod;
}

unsigned int PolygonalDrawable::numVertices() const
{
    if (m_geometry)
    {
        return m_geometry->vertices()->size();
    } else {
        return 0;
    }
}
