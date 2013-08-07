
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
    if (!m_pass) return 0;

    bool status = m_pass->isActive();

    m_pass->setActive(true);
    m_pass->applyIfActive();

    FrameBufferObject * fbo = m_pass->output();

    if (x >= fbo->width() || y >= fbo->height())
        return 0;

    fbo->bind();
    glFlush();
    glFinish();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    float data[4];
    glReadPixels(x, fbo->height()-y, 1, 1, GL_RGBA, GL_FLOAT, data);
    fbo->release();

    m_pass->setActive(status);

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

void CoordinateProvider::assignPass(RenderingPass * pass)
{
    m_pass = pass;
}

void CoordinateProvider::resize(const int width, const int height)
{
    if (m_pass)
        m_pass->resize(width, height);
}

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
            }
            return true;
        });
}