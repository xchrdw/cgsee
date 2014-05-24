
#include <core/navigation/abstractnavigation.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <QGLWidget>
#include <QDebug>

#include <core/camera.h>
#include <core/navigation/viewhistory.h>

#include <signalzeug/Signal.h>


const float AbstractNavigation::TIMER_MS = 1000.f / 60.f;

static const float DURATION = 333.f;

AbstractNavigation::AbstractNavigation(Camera * camera)
    : m_width(camera->viewport().x)
    , m_height(camera->viewport().y)
    , m_BBRadius(0)
    , m_fovy(camera->fovy())
    , m_viewmatrix(camera->view())
    , m_viewHistory()
    , m_camera(camera)
    , m_canvas(0)
    , m_timer()
    , m_timer_requests(0)
    , m_animation_active(false)
{
    m_frontView = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
    signalzeug::Signal<> viewChanged;
}


AbstractNavigation::~AbstractNavigation()
{
}


const glm::mat4 & AbstractNavigation::viewMatrix()
{
    return m_viewmatrix;
}


void AbstractNavigation::updateCamera()
{
    m_camera->setFovy(m_fovy);
    m_camera->setView(m_viewmatrix);
    if(m_canvas)
        m_canvas->repaint();
    onCameraChanged();
}


void AbstractNavigation::onCameraChanged() { }


void AbstractNavigation::reset()
{
    m_viewmatrix = glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    updateCamera();
}


void AbstractNavigation::keyPressEvent(QKeyEvent *event) { }
void AbstractNavigation::keyReleaseEvent(QKeyEvent *event) { }

void AbstractNavigation::mouseMoveEvent(QMouseEvent * event) { }
void AbstractNavigation::mousePressEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseReleaseEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseDoubleClickEvent(QMouseEvent * event) { }

void AbstractNavigation::wheelEvent(QWheelEvent * event)
{
    m_fovy -= (event->delta() * 0.1); //sensitivity
    m_fovy = glm::clamp(m_fovy, 1.0f, 180.0f);
    updateCamera();

    // @TODO  run a timer that is reset in every wheel event and when it times out, we know the user stopped scrolling: then saveViewHistory!
    // now: for long wheelEvents, saveViewHistory is called multiple times.

    saveViewHistory();
}


void AbstractNavigation::setViewPort( const int width, const int height )
{
    m_width = width;
    m_height = height;
}

void AbstractNavigation::setCanvas(QWidget * canvas)
{
    m_canvas = canvas;
}

void AbstractNavigation::setCamera(Camera * camera)
{
    m_camera = camera;
    updateCamera();
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
* stop if all requests were stopped.
*/
void AbstractNavigation::stopTimer()
{
    m_timer_requests = glm::max(0, m_timer_requests-1);

    if(m_timer_requests == 0) {
        m_timer.stop();
    }
}


void AbstractNavigation::timerEvent(QTimerEvent * event)
{
    if (m_animation_active) {
        m_animation_progress += TIMER_MS / DURATION;
        if (m_animation_progress < 1.f) {
            updateTransition();
        } else {
            finishTransition();
        }
    }
    else {
        onTimerEvent(); // handled by super class
    }
}

void AbstractNavigation::updateTransition()
{
    float step = glm::smoothstep(0.f, 1.f, m_animation_progress);
    glm::mat4 translation = glm::translate(glm::mix(m_old_pos, m_new_pos, step));
    glm::mat4 rotation = glm::mat4_cast(glm::slerp(m_old_rotation, m_new_rotation, step));
    m_viewmatrix = translation * rotation;
    m_fovy = m_old_fovy + step * (m_new_fovy - m_old_fovy);
    updateCamera();
}

void AbstractNavigation::finishTransition()
{
    m_animation_active = false;
    stopTimer();
    m_viewmatrix = glm::translate(m_new_pos) * glm::mat4_cast(m_new_rotation);
    m_fovy = m_new_fovy;
    setFromMatrix(m_viewmatrix);
    updateCamera();
}

void AbstractNavigation::onTimerEvent() { }


bool AbstractNavigation::isTimerRunning()
{
    return m_timer.isActive();
}

void AbstractNavigation::loadView(const glm::mat4 & new_viewmatrix, bool save_history)
{
    m_old_rotation = glm::quat_cast(m_viewmatrix);
    m_new_rotation = glm::quat_cast(new_viewmatrix);

    m_old_pos = glm::column(m_viewmatrix, 3).xyz();
    m_new_pos = glm::column(new_viewmatrix, 3).xyz();

    m_old_fovy = m_fovy;

    if(save_history){
        m_new_fovy = m_fovy;
        saveViewHistory();
    } else {
        m_new_fovy = m_viewHistory->getFovy();
    }

    m_animation_progress = 0;
    m_animation_active = true;

    if(!isTimerRunning()) {
        startTimer();
    }
}

void AbstractNavigation::saveViewHistory()
{
    if(!m_viewHistory->isEqualViewMatrix(m_viewmatrix) || !m_viewHistory->isEqualFovy(m_fovy)){
        m_viewHistory = new ViewHistory(m_viewHistory, m_viewmatrix, m_fovy);
        qDebug() << "save #" <<  m_viewHistory->getTimestamp() << " / " << m_viewHistory->getSize() << " views in history.";
    }
}

void AbstractNavigation::undoViewHistory()
{
    // if not reached the oldest history element
    if(!m_viewHistory->isFirst()){
        if (m_viewHistory->isLast()) {

            // save last object before undo
            saveViewHistory();
        }
        m_viewHistory = m_viewHistory->getPrevious();
        loadView(m_viewHistory->getPrevious()->getViewMatrix(), false);
        qDebug() << "undo #" << m_viewHistory->getTimestamp() << " / " << m_viewHistory->getSize() << " views in history.";
    }
}

void AbstractNavigation::redoViewHistory()
{
    // if not reached the youngest history element
    if(!m_viewHistory->isLast()){
        m_viewHistory = m_viewHistory->getNext();
        loadView(m_viewHistory->getViewMatrix(), false);
        qDebug() << "redo #" << m_viewHistory->getTimestamp() << " / " << m_viewHistory->getSize() << " views in history.";
    }
}

void AbstractNavigation::setFromMatrix(const glm::mat4 & view)
{
    m_viewmatrix = view;
}

glm::mat4 AbstractNavigation::defaultView()
{
    return frontview();
}

glm::mat4 AbstractNavigation::frontview()
{
    return m_frontView;
}

glm::mat4 AbstractNavigation::rightview()
{
    return frontview() * glm::rotate(-90.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractNavigation::backview()
{
    return frontview() * glm::rotate(180.0f, glm::vec3(0,1,0));
}

glm::mat4 AbstractNavigation::leftview()
{
    return frontview() * glm::rotate(90.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractNavigation::topview()
{
    return frontview() * glm::rotate(-90.f, glm::vec3(1,0,0));
}

glm::mat4 AbstractNavigation::bottomview()
{
    return frontview() * glm::rotate(90.f, glm::vec3(1,0,0));
}

glm::mat4 AbstractNavigation::topRightView()
{
    return frontview() * glm::rotate(30.f, glm::vec3(1,0,0)) * glm::rotate(45.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractNavigation::bottomLeftView()
{
    return frontview() * glm::rotate(-30.f, glm::vec3(1,0,0)) * glm::rotate(-45.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractNavigation::randomView()
{
    float x = static_cast <float> (rand() % 180);
    float y = static_cast <float> (rand() % 180);
    float z = static_cast <float> (rand() % 180);
    return frontview() * glm::rotate(x, glm::vec3(1,0,0)) * glm::rotate(y, glm::vec3(0,1,0)) * glm::rotate(z, glm::vec3(0,0,1));
}

glm::mat4 AbstractNavigation::sceneTransform()
{
    return m_sceneTransform;
}

// gets called at every program start and when a new file is opened in the viewer
void AbstractNavigation::sceneChanged(Group * scene)
{
    AxisAlignedBoundingBox bb = scene->boundingBox();

    m_BBRadius = bb.radius();

    m_frontView = glm::lookAt(bb.center() + glm::vec3(0.f, 0.f, bb.radius()*2.5), bb.center(), glm::vec3(0.f, 1.f, 0.f));
    setFromMatrix(topRightView());
    updateCamera();

    saveViewHistory();
}

float AbstractNavigation::getBBRadius(){
    return m_BBRadius;
}

void AbstractNavigation::setBBRadius(float radius){
    m_BBRadius = radius;
}

// Workaround to avoid invisible objects due to wrong clipping planes
// all scenes are scaled to a radius of 5
void AbstractNavigation::rescaleScene( Group * scene )
{
    AxisAlignedBoundingBox bb = scene->boundingBox();

    glm::mat4 scale_matrix = glm::scale(glm::vec3(5.0f / bb.radius()));
    m_sceneTransform = scale_matrix * scene->transform();
    scene->setTransform(m_sceneTransform);
}
