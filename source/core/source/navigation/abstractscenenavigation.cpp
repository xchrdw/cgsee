
#include <core/navigation/abstractscenenavigation.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <QGLWidget>

#include <core/camera.h>

const float AbstractSceneNavigation::TIMER_MS = 1000.f / 60.f;

static const float DURATION = 333.f;

AbstractSceneNavigation::AbstractSceneNavigation(Camera * camera)
    : m_BBRadius(0)
    , m_fovy(camera->fovy())
    , m_viewmatrix(camera->view())
    , m_camera(camera)
    , m_timer()
    , m_timer_requests(0)
    , m_animation_active(false)
{
    m_frontView = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0), glm::vec3(0.f, 1.f, 0.f));
}


AbstractSceneNavigation::~AbstractSceneNavigation()
{
}


const glm::mat4 & AbstractSceneNavigation::viewMatrix()
{
    return m_viewmatrix;
}


void AbstractSceneNavigation::updateCamera()
{
    m_camera->setFovy(m_fovy);
    m_camera->setView(m_viewmatrix);
    if(m_canvas)
        m_canvas->repaint();
    onCameraChanged();
}


void AbstractSceneNavigation::onCameraChanged() { }


void AbstractSceneNavigation::reset()
{
    m_viewmatrix = glm::lookAt(
        glm::vec3( 0.f, 0.f,-2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    updateCamera();
}

void AbstractSceneNavigation::wheelEvent(QWheelEvent * event)
{
    m_fovy += (event->delta() * 0.1); //sensitivity
    m_fovy = glm::clamp(m_fovy, 1.0f, 180.0f);
    updateCamera();
}

void AbstractSceneNavigation::setCamera(Camera * camera)
{
    m_camera = camera;
    updateCamera();
}


/**
* starts the timer. if it's called multiple times, you need
* to make the same number of stop calls to stop it.
*/
void AbstractSceneNavigation::startTimer()
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
void AbstractSceneNavigation::stopTimer()
{
    m_timer_requests = glm::max(0, m_timer_requests-1);

    if(m_timer_requests == 0) {
        m_timer.stop();
    }
}


void AbstractSceneNavigation::timerEvent(QTimerEvent * event)
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

void AbstractSceneNavigation::updateTransition()
{
    float step = glm::smoothstep(0.f, 1.f, m_animation_progress);
    glm::mat4 translation = glm::translate(glm::mix(m_old_pos, m_new_pos, step));
    glm::mat4 rotation = glm::mat4_cast(glm::slerp(m_old_rotation, m_new_rotation, step));
    m_viewmatrix = translation * rotation;
    updateCamera();
}

void AbstractSceneNavigation::finishTransition()
{
    m_animation_active = false;
    stopTimer();
    m_viewmatrix = glm::translate(m_new_pos) * glm::mat4_cast(m_new_rotation);
    setFromMatrix(m_viewmatrix);
    updateCamera();
}

void AbstractSceneNavigation::onTimerEvent() { }


bool AbstractSceneNavigation::isTimerRunning()
{
    return m_timer.isActive();
}

void AbstractSceneNavigation::loadView(const glm::mat4 & new_viewmatrix)
{
    m_old_rotation = glm::quat_cast(m_viewmatrix);
    m_new_rotation = glm::quat_cast(new_viewmatrix);

    m_old_pos = glm::column(m_viewmatrix, 3).xyz();
    m_new_pos = glm::column(new_viewmatrix, 3).xyz();

    m_animation_progress = 0;
    m_animation_active = true;

    if(!isTimerRunning()) {
        startTimer();
    }
}

void AbstractSceneNavigation::setFromMatrix(const glm::mat4 & view)
{
    m_viewmatrix = view;
}

glm::mat4 AbstractSceneNavigation::defaultView()
{
    return frontview();
}

glm::mat4 AbstractSceneNavigation::frontview()
{
    return m_frontView;
}

glm::mat4 AbstractSceneNavigation::rightview()
{
    return frontview() * glm::rotate(-90.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractSceneNavigation::backview()
{
    return frontview() * glm::rotate(180.0f, glm::vec3(0,1,0));
}

glm::mat4 AbstractSceneNavigation::leftview()
{
    return frontview() * glm::rotate(90.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractSceneNavigation::topview()
{
    return frontview() * glm::rotate(-90.f, glm::vec3(1,0,0));
}

glm::mat4 AbstractSceneNavigation::bottomview()
{
    return frontview() * glm::rotate(90.f, glm::vec3(1,0,0));
}

glm::mat4 AbstractSceneNavigation::topRightView()
{
    return frontview() * glm::rotate(30.f, glm::vec3(1,0,0)) *  glm::rotate(45.f, glm::vec3(0,1,0));
}

glm::mat4 AbstractSceneNavigation::sceneTransform()
{
    return m_sceneTransform;
}

void AbstractSceneNavigation::sceneChanged(Group * scene)
{
    AxisAlignedBoundingBox bb = scene->boundingBox();

    m_BBRadius = bb.radius();

    m_frontView = glm::lookAt(bb.center() + glm::vec3(0.f, 0.f, bb.radius()*2.5), bb.center(), glm::vec3(0.f, 1.f, 0.f));
    setFromMatrix(topRightView());
    updateCamera();
}

float AbstractSceneNavigation::getBBRadius(){
    return m_BBRadius;
}

void AbstractSceneNavigation::setBBRadius(float radius){
    m_BBRadius = radius;
}

// Workaround to avoid invisible objects due to wrong clipping planes
// all scenes are scaled to a radius of 5
void AbstractSceneNavigation::rescaleScene( Group * scene )
{
    AxisAlignedBoundingBox bb = scene->boundingBox();

    glm::mat4 scale_matrix = glm::scale(glm::vec3(5.0f / bb.radius()));
    m_sceneTransform = scale_matrix * scene->transform();
    scene->setTransform(m_sceneTransform);
}
