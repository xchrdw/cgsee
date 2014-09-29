
#include <core/scenegraph/triangleobject.h>

#include <core/aabb.h>
#include <core/program.h>


TriangleObject::TriangleObject(const QString & name)
:   Node( name )
,   m_triangles ( new std::vector<glm::vec3>() )
{
}

TriangleObject::~TriangleObject()
{
    delete m_triangles;
}

const AxisAlignedBoundingBox TriangleObject::boundingBox() const
{
    if( m_aabb.valid() )
        return m_aabb;

    if ( m_triangles->size() == 0 )
        return m_aabb = AxisAlignedBoundingBox();

    for ( auto vertex : *m_triangles ) {
        m_aabb.extend( vertex );
    }
    return m_aabb;
}

const AxisAlignedBoundingBox TriangleObject::boundingBox(glm::mat4 transform) const
{
    AxisAlignedBoundingBox aabb = AxisAlignedBoundingBox();
    glm::mat4 newTransform;

    if ( m_triangles->size() == 0 )
        return aabb;

    if (m_rf == RF_Absolute) {
        newTransform = transform;
    } else {
        newTransform = this->transform() * transform;
    }

    for ( auto vertex : *m_triangles ) {
        glm::vec4 transformedVertex = newTransform * glm::vec4(vertex, 1.0f);
        aabb.extend( transformedVertex.xyz() * (1.0f / transformedVertex.w) );
    }
    return aabb;
}

void TriangleObject::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void TriangleObject::draw(const Program & program, const glm::mat4 & transform)
{
    // TriangleObject is only a helper class for the raytracer, no need to implement a draw function
}

p_TriangleList TriangleObject::triangles()
{
    return m_triangles;
}
