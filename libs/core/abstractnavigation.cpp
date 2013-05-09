#include "abstractnavigation.h"
#include "camera.h"

AbstractNavigation::AbstractNavigation(Camera *camera) 
    : m_camera(camera)
    , m_viewMatrix(camera->view())
    , m_width(camera->viewport().x)
    , m_height(camera->viewport().y)
    , m_saved_views(12)
{
    m_saved_views[0] = glm::lookAt(glm::vec3(0.f, 0.f,-2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[1] = glm::lookAt(glm::vec3(2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[2] = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    m_saved_views[3] = glm::lookAt(glm::vec3(-2.f, 0.f, 0.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    for (int i = 4; i < m_saved_views.size(); i++)
    {
        m_saved_views[i] = m_saved_views[0];
    }
}


AbstractNavigation::~AbstractNavigation(void) 
{
}

void AbstractNavigation::reset()
{
    m_viewMatrix = glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    m_camera->setView(m_viewMatrix);
}

void AbstractNavigation::keyPressEvent( QKeyEvent *event ) { }
void AbstractNavigation::keyReleaseEvent( QKeyEvent *event ) { }

void AbstractNavigation::mouseMoveEvent(QMouseEvent * event) { }
void AbstractNavigation::mousePressEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseReleaseEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseDoubleClickEvent(QMouseEvent * event) { }

void AbstractNavigation::wheelEvent(QWheelEvent *event) { }

void AbstractNavigation::setViewPort( const int width, const int height )
{
    m_width = width;
    m_height = height;
}

void AbstractNavigation::setFovy(float fovy) { }
float AbstractNavigation::getFovy(){
    //Return Standard FOV if Navigation hasn't implemented a FOV manipulator
    return 45.0f;
}

void AbstractNavigation::saveView(int viewnr)
{
    m_saved_views[viewnr] = m_viewMatrix;
}

void AbstractNavigation::loadView(int viewnr)
{
    m_camera->setView(m_saved_views[viewnr]);
    m_viewMatrix = m_saved_views[viewnr];
}



