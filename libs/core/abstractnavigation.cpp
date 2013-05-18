#include "abstractnavigation.h"
#include "camera.h"
#include <QGLWidget>



AbstractNavigation::AbstractNavigation(Camera *camera) 
    : m_camera(camera)
    , m_viewMatrix(camera->view())
    , m_width(camera->viewport().x)
    , m_height(camera->viewport().y)
    , m_canvas(0)
    , m_timer()
    , m_timer_requests(0)
    , m_animation_active(false)
{
    
}


AbstractNavigation::~AbstractNavigation(void) 
{
}


const glm::mat4 AbstractNavigation::viewMatrix()
{
    return m_viewMatrix;
}


void AbstractNavigation::updateCamera()
{
    m_camera->setView(m_viewMatrix);
    if(m_canvas)
        m_canvas->repaint();
    onCameraChanged();
}


void AbstractNavigation::onCameraChanged() { }


void AbstractNavigation::reset()
{
    m_viewMatrix = glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    updateCamera();
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

void AbstractNavigation::setCanvas( QWidget * canvas )
{
    m_canvas = canvas;
}


/**
* starts the timer. if it's called multiple times, you need 
* to make the same number of stop calls to stop it.
*/
void AbstractNavigation::startTimer()
{
    m_timer_requests++;
    if(!m_timer.isActive()) {
        m_timer.start(TIMER_MS, this);
    }
}

/**
* counts how often the timer was requested and only 
* stop if all requests were stopped
*/
void AbstractNavigation::stopTimer()
{
    m_timer_requests = glm::max(0, m_timer_requests-1);

    if(m_timer_requests == 0) {
        m_timer.stop();
    }
}

void AbstractNavigation::timerEvent( QTimerEvent* event ) {
    if (m_animation_active) {
        m_animation_progress += TIMER_MS/333.0f;
        if (m_animation_progress > 1) {
            m_animation_active = false;
            stopTimer();
            m_viewMatrix = m_newMatrix;
            setFromMatrix(m_viewMatrix);
            updateCamera();
        } else {
            m_viewMatrix = (1-m_animation_progress) * m_oldMatrix + m_animation_progress * m_newMatrix; // TODO replace with quaternion interpolation
            updateCamera();
        }
    }
    else {
        onTimerEvent(); // notify super class
    }
}

void AbstractNavigation::onTimerEvent() { }


bool AbstractNavigation::isTimerRunning()
{
    return m_timer.isActive();
}

void AbstractNavigation::loadView( glm::mat4 viewmatrix )
{
    m_oldMatrix = m_viewMatrix;
    m_newMatrix = viewmatrix;
    
    m_animation_progress = 0;
    m_animation_active = true;
    

    if(!isTimerRunning())
        startTimer();
}

void AbstractNavigation::setFromMatrix( glm::mat4 view ) { }






