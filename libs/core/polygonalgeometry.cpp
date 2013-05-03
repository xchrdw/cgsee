
#include "polygonalgeometry.h"

#include "aabb.h"


PolygonalGeometry::PolygonalGeometry(const QString & name)
:   Node(name)
,   m_mode(GL_TRIANGLES)
{
}

PolygonalGeometry::~PolygonalGeometry()
{
}

const GLenum PolygonalGeometry::mode() const
{
    return m_mode;
}

void PolygonalGeometry::setMode(const GLenum mode)
{
    m_mode = mode;
}

const t_vec3s & PolygonalGeometry::vertices() const
{
    return m_vertices;
}

t_vec3s & PolygonalGeometry::vertices()
{
    invalidateBoundingBox();
    return m_vertices;
}

const t_vec3s & PolygonalGeometry::normals() const
{
    return m_normals;
}

t_vec3s & PolygonalGeometry::normals()
{
    return m_normals;
}

const t_vec2s & PolygonalGeometry::texcs() const
{
    return m_texcs;
}
t_vec2s & PolygonalGeometry::texcs()
{
    return m_texcs;
}

const t_uints & PolygonalGeometry::indices() const
{
    return m_indices;
}

t_uints & PolygonalGeometry::indices()
{
    return m_indices;
}

const AxisAlignedBoundingBox PolygonalGeometry::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    t_vec3s::const_iterator i(m_vertices.begin());
    const t_vec3s::const_iterator iEnd(m_vertices.end());

    for(; i != iEnd; ++i)
        m_aabb.extend(*i);

    return m_aabb;
}

void PolygonalGeometry::retrieveNormals()
{
    if(m_vertices.isEmpty())
        return;

    if(!m_normals.isEmpty())
        qDebug("Normals of %s will be replaced.", qPrintable(name()));

    assert(m_indices.size() % 3 == 0);

    unsigned int i0, i1, i2;
    glm::vec3 v[3], a, b, n;

    // check size and resize if necassary

    if(m_normals.size() != m_vertices.size())
        m_normals.resize(m_vertices.size());

    for(int i = 0; i < m_indices.size(); i += 3)
    {
        i0 = m_indices[i + 0];
        i1 = m_indices[i + 1];
        i2 = m_indices[i + 2];

        a = glm::normalize(m_vertices[i2] - m_vertices[i0]);
        b = glm::normalize(m_vertices[i1] - m_vertices[i0]);

        n = glm::cross(a, b);

        m_normals[i0] = n;
        m_normals[i1] = n;
        m_normals[i2] = n;
    }
}

void PolygonalGeometry::draw(
    Program * program
,   const glm::mat4 & transform)
{
}
