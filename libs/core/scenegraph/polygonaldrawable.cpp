#include <GL/glew.h>

#include "polygonaldrawable.h"

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "nulliterator.h"
#include "polygonalgeometry.h"

static const QString TRANSFORM_UNIFORM( "transform" );

// TODO: wieder rueckgaengig machen...
// PolygonalDrawable::PolygonalDrawable( DataBlockRegistry & registry, const QString & name )
PolygonalDrawable::PolygonalDrawable( const QString & name )
:   Node( name )
,   m_vao( -1 )
,   m_geometry( nullptr )
,   m_mode( GL_TRIANGLES )
{
}

PolygonalDrawable::~PolygonalDrawable()
{
    deleteBuffers();
}

t_nodeIterator PolygonalDrawable::begin()
{
    return t_nodeIterator( new NullIterator() );
}

t_nodeIterator PolygonalDrawable::end()
{
    return t_nodeIterator( new NullIterator() );
}

void PolygonalDrawable::setGeometry( t_geometryP geometry )
{
    if (m_geometry != nullptr)
        deleteBuffers();

    m_geometry = geometry;
}

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    if(m_geometry == nullptr)
        return AxisAlignedBoundingBox();

    // TODO: Hier muss was effizienteres her!
    for( const auto pos : m_geometry->vertices() ){
        m_aabb.extend( pos );
    }

    return m_aabb;
}

void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    if(!m_geometry || m_geometry->indices().empty())
        return;

    if(m_arrayBOsByAttribute.empty() || m_elementArrayBOs.empty())
        initialize(program);

    program.use();
    program.setUniform(TRANSFORM_UNIFORM, transform);

    glBindVertexArray(m_vao);                                                                  
    glError();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
 
    for( const auto & bo : m_elementArrayBOs )
        bo->draw( mode() );

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);
    glError();

    program.release();
}

void PolygonalDrawable::initialize(const Program & program)
{
    assert(m_geometry);

    deleteBuffers();

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);                                                                  
    glError();

    // setup element array buffers

    BufferObject * indexBO(new BufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW));
    indexBO->data<GLuint>(m_geometry->indices(), GL_UNSIGNED_INT, 1);

    m_elementArrayBOs.push_back(indexBO);

    // setup array buffers

    BufferObject * vertexBO(new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
    vertexBO->data<glm::vec3>(m_geometry->vertices(), GL_FLOAT, 3);
    
    m_arrayBOsByAttribute["a_vertex"] = vertexBO;

    // TODO: the geometry should provide this information.

    if(!m_geometry->normals().isEmpty())
    {
        BufferObject * normalBO(new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
        normalBO->data<glm::vec3>(m_geometry->normals(), GL_FLOAT, 3);

        m_arrayBOsByAttribute["a_normal"] = normalBO;
    }

    // bind all buffers to their attributes

    t_bufferObjectsByAttribute::const_iterator i(m_arrayBOsByAttribute.begin());
    const t_bufferObjectsByAttribute::const_iterator iEnd(m_arrayBOsByAttribute.end());

    for(; i != iEnd; ++i)
        i.value()->bind(program.attributeLocation(i.key()));

    glBindVertexArray(0);
    glError();
}

void PolygonalDrawable::deleteBuffers()
{
    qDeleteAll( m_elementArrayBOs );
    qDeleteAll( m_arrayBOsByAttribute );

    if(-1 != m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
        glError();

        m_vao = -1;
    }
}
