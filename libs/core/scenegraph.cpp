#include "scenegraph.h"

#include "datacore/abstractdata.h"
#include "group.h"
#include "polygonaldrawable.h"

SceneGraph::SceneGraph( DataBlockRegistry &registry ) 
: m_registry( registry )
, m_root( "root" )
{

}

Group * SceneGraph::createGroup( Group * parent, const QString & name )
{
    if (parent == nullptr)
        parent = &m_root;
    
    Group * group = new Group( name );
    group->setParent( parent );
    return group;
}

PolygonalDrawable * SceneGraph::createPolygonalDrawable( Group * parent, const QString & name )
{
    if (parent == nullptr)
        parent = &m_root;
        
    PolygonalDrawable *pd = new PolygonalDrawable( m_registry, name );
    pd->setParent( parent );
    return pd;
}

const Group & SceneGraph::root() const
{
    return m_root;
}

Group & SceneGraph::root()
{
    return m_root;
}

