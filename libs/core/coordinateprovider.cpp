
#include "coordinateprovider.h"
#include "idpainter.h"

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/scenetraverser.h>


CoordinateProvider::CoordinateProvider()
:   m_painter(nullptr)
,   m_rootNode(nullptr)
{

}

CoordinateProvider::~CoordinateProvider()
{

}

void CoordinateProvider::assignCamera(Camera * camera)
{
    delete m_painter;
    m_painter = new IdPainter(camera);
}

void CoordinateProvider::assignScene(Group * rootNode)
{
    m_rootNode = rootNode;
    initialize();
    m_painter->assignScene(rootNode);
}

// #include <iostream>
void CoordinateProvider::initialize()
{
    m_nodes.clear();

    SceneTraverser traverser;
    traverser.traverse(*m_rootNode, [&] (Node & node) 
        {
            if (PolygonalDrawable * drawable = dynamic_cast <PolygonalDrawable *> (& node))
            {
                node.setId(this->m_nodes.size()*100);
                this->m_nodes.push_back(&node);
                // std::cout << m_nodes.size() << std::endl;
                // std::cout << node.id() << std::endl;
            }
            return true;
        });
}