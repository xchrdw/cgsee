
#include "coordinateprovider.h"
#include "framebufferobject.h"

#include "rendering/coloridpass.h"

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/scenetraverser.h>


CoordinateProvider::CoordinateProvider()
:   m_pass(nullptr)
,   m_rootNode(nullptr)
{

}

CoordinateProvider::~CoordinateProvider()
{

}

unsigned int CoordinateProvider::objID(int x, int y)
{
    if (!m_pass)
    {
        return 0;
    }
    int i = 0;
    m_pass->setActive(true);
    m_pass->applyIfActive();

    FrameBufferObject * fbo = m_pass->output();

    fbo->bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    float data[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data);
    fbo->release();

    m_pass->setActive(false);

    return 255*data[0] + 255*data[1]*255 + 255*data[2]*255*255 + 255*data[3]*255*255*255;
}

void CoordinateProvider::assignCamera(Camera * camera)
{
    delete m_pass;
    m_pass = new ColorIdPass(camera);
}

void CoordinateProvider::assignScene(Group * rootNode)
{
    m_rootNode = rootNode;
    initialize();
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
                node.setId(this->m_nodes.size());
                this->m_nodes.push_back(&node);
                // std::cout << m_nodes.size() << std::endl;
                // std::cout << node.id() << std::endl;
            }
            return true;
        });
}