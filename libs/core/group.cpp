
#include "group.h"

#include "polygonaldrawable.h"
#include "program.h"


Group::Group( const QString & name )
: Node(name)
{
    
}

Group::~Group()
{
    
}

void Group::draw( const Program & program, const glm::mat4 & transform )
{
    if( !isOn() )
        return;
    
    for( auto object : children() )
    {
        Node * node = dynamic_cast<Node*>( object );
        if(RF_Absolute == m_rf)
            node->draw(program, this->transform());
        else
            node->draw(program, transform * this->transform());
    }
}

const bool Group::contains( QObject * node ) const
{
    return children().contains( node );
}

// void Group::insert(
//     const Group::t_nodes::iterator & before
// ,   Group * group)
// {
//     return insert(before, dynamic_cast<Node *>(group));
// }
// 
// void Group::insert(
//     const Group::t_nodes::iterator & before
// ,   PolygonalDrawable * drawable)
// {
//     return insert(before, dynamic_cast<Node *>(drawable));
// }
// 
// void Group::insert(
//     const Group::t_nodes::iterator & before
// ,   Node * node)
// {
//     if(!node)
//         return;
// 
//     if(!contains(node))
//         node->parents().insert(this);
// 
//     m_children.insert(before, node);
// }

// void Group::prepend(Group * group)
// {
//     return prepend(dynamic_cast<Node *>(group));
// }
// 
// void Group::prepend(PolygonalDrawable * drawable)
// {
//     return prepend(dynamic_cast<Node *>(drawable));
// }
// 
// void Group::prepend(Node * node)
// {
//     if(!node)
//         return;
// 
//     if(!contains(node))
//         node->parents().insert(this);
// 
//     m_children.push_front(node);
// }

void Group::append( Node * node )
{
    return append( dynamic_cast<QObject *>(node) );
}

void Group::append( QObject * node )
{
    node->setParent( this );
}

// void Group::removeFirst()
// {
//     if(m_children.empty())
//         return;
// 
//     Node * node(m_children.front());
// 
//     node->parents().remove(this);
//     m_children.pop_front();
// 
//     if(node->parents().empty())
//         delete node;
// }
// 
// void Group::removeLast()
// {
//     if(m_children.empty())
//         return;
// 
//     Node * node(m_children.back());
// 
//     node->parents().remove(this);
//     m_children.pop_back();
// 
//     if(node->parents().empty())
//         delete node;
// }

void Group::remove( Node * node )
{
    contains( dynamic_cast<QObject *>(node) );
}

void Group::remove( QObject * node )
{
    if (contains(node))
        delete node;
}

const AxisAlignedBoundingBox Group::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

//     t_nodes::const_iterator i(m_children.begin());
//     const t_nodes::const_iterator iEnd(m_children.end());

    if(RF_Relative == m_rf){
        for( auto i : children() ){
            Node * node = dynamic_cast<Node*>( i );
            m_aabb.extend(node->boundingBox() * transform());
        }
    }
    else{
        for( auto i : children() ){
            Node * node = dynamic_cast<Node*>( i );
            m_aabb.extend(node->boundingBox());
        }
    }
    
    return m_aabb;
}

Group * Group::asGroup()
{
    return this;
}