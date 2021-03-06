
#include <core/scenegraph/drawvisitor.h>

#include <core/scenegraph/node.h>


DrawVisitor::DrawVisitor(const Program * p, glm::mat4 t) 
: m_program(p)
, m_transform(t)
{}

bool DrawVisitor::operator() (Node & node)
{
    if( node.hidden() )
        return false;
    node.draw(*m_program, m_transform);
    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return true;
}
