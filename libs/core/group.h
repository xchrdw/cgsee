#pragma once

#include "declspec.h"

#include "node.h"

class PolygonalDrawable;
class Program;

class CGSEE_API Group : public Node
{
public:
    Group( const QString & name );
    virtual ~Group();

    virtual Group * asGroup();

    virtual void draw(
        const Program & program
    ,   const glm::mat4 & transform);


    // child handling

    // TODO: Nach Node verschieben.
    const bool contains( QObject * node ) const;

    void append( Node * group );
    void remove( Node * node );
    
    virtual const AxisAlignedBoundingBox boundingBox() const;
    
//     void prepend(Group * group);
//     void prepend(PolygonalDrawable * drawable);
//     
//     void insert(
//         const t_nodes::iterator & before
//     ,   Group * group);
//     void insert(
//         const t_nodes::iterator & before
//     ,   PolygonalDrawable * drawable);
//
//     both removals delete the node if parents are empty
//     void removeFirst();
//     void removeLast ();
// 
//     const t_nodes & children() const;
//     t_nodes & children();

protected:
    void append( QObject * node );
    void remove( QObject * node );
    
//     void prepend(Node * node);
//     void insert( const t_nodes::iterator & before, Node * node );
};
