
#include "coordinateprovider.h"
#include "framebufferobject.h"

#include "rendering/coloridpass.h"

#include <core/scenegraph/node.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/scenegraph/scenetraverser.h>

#include <core/camera.h>


CoordinateProvider::CoordinateProvider()
:   m_pass(nullptr)
// ,   m_rootNode(nullptr)
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

glm::dvec3 CoordinateProvider::pointAt(int x, int y)
{
    if (!m_pass) return glm::dvec3(0.0f,0.0f,0.0f);

    bool status = m_pass->isActive();

    m_pass->setActive(true);
    m_pass->applyIfActive();

    FrameBufferObject * fbo = m_pass->output();

    if (x >= fbo->width() || y >= fbo->height())
        return glm::dvec3(0.0f,0.0f,0.0f);

    fbo->bind();
    glFlush();
    glFinish();
    // glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    float z;
    y = fbo->height()-y;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    fbo->release();

    m_pass->setActive(status);

    return unproject((float) x, (float) y, z);
}

glm::dvec3 CoordinateProvider::unproject(float x, float y, float z)
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glm::mat4 viewMat = m_pass->camera()->view();
    glm::mat4 projectionMat = m_pass->camera()->projection();

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            modelview[i*4+j] = viewMat[i][j];
            projection[i*4+j] = projectionMat[i][j];
        }
    }

    viewport[0] = viewport[1] = 0;
    viewport[2] = m_pass->camera()->viewport().x;
    viewport[3] = m_pass->camera()->viewport().y;

    glm::dvec3 position;    
    gluUnProject( x, y, z, modelview, projection, viewport, &position.x, &position.y, &position.z );

    return position;
}

void CoordinateProvider::assignCamera(Camera * camera)
{
    delete m_pass;
    m_pass = new ColorIdPass(camera);
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
