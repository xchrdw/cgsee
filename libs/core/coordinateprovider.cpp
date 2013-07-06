
#include "coordinateprovider.h"

#include <core/scenegraph/node.h>
#include <core/scenegraph/scenetraverser.h>


CoordinateProvider::CoordinateProvider()
:   m_painter(nullptr)
,   m_rootNode(nullptr)
{

}

CoordinateProvider::~CoordinateProvider()
{

}

void CoordinateProvider::assignScene(Node * rootNode)
{
    m_rootNode = rootNode;
    initialize();
}


void CoordinateProvider::initialize()
{
    m_nodes.clear();

    SceneTraverser traverser;
    traverser.traverse(*m_rootNode, [&] (Node & node) 
        {
            node.setId(this->m_nodes.size());
            this->m_nodes.push_back(&node);
            return true;
        });
}