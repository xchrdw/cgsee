
#include <core/camera/abstractcamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <core/camera/projection.h>
#include <core/viewfrustum.h>
#include <core/program.h>

//takes ownership of projection
AbstractCamera::AbstractCamera(const QString & name, Projection * projection)
:   Group(name)
,   m_invalid(true)
,   m_viewFrustum(new ViewFrustum(this))
,   m_projection(projection)
,   m_view(glm::mat4())
,   m_viewChanged(signalzeug::Signal<>())
{
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
}

AbstractCamera::AbstractCamera(const AbstractCamera & camera)
:   Group(camera.m_name)
,   m_invalid(true)
,   m_viewFrustum(new ViewFrustum(this))
,   m_projection(new Projection(*(camera.m_projection))) //TODO find out what happens here
,   m_view(camera.m_view)
{
    m_rf = camera.m_rf;
}

AbstractCamera::~AbstractCamera()
{
    delete m_projection;
    delete m_viewFrustum;
}

glm::mat4 AbstractCamera::viewProjection()
{
    update();
    return m_viewProjection;
}

//takes ownership of projection
void AbstractCamera::setProjection(Projection * projection)
{
    if(projection == m_projection)
        return;

    delete m_projection;
    m_projection = projection;
    invalidate();
}
Projection * AbstractCamera::getProjection() const
{
    return m_projection;
}

void AbstractCamera::setViewport(const glm::ivec2 & size)
{
    m_projection->setViewport(size);
    invalidate();
}
void AbstractCamera::setViewport(const int width, const int height)
{
    m_projection->setViewport(width, height );
    invalidate();
}
const glm::ivec2 & AbstractCamera::viewport() const
{
    return m_projection->viewport();
}
const float AbstractCamera::aspect() const
{
    return m_projection->aspect();
}

void AbstractCamera::setFovy(const float fovy)
{
    m_projection->setFovy(fovy);
    invalidate();
}
const float AbstractCamera::fovy() const
{
    return m_projection->fovy();
}
void AbstractCamera::setZNear(const float z)
{
    m_projection->setZNear(z);
    invalidate();
}
const float AbstractCamera::zNear() const
{
    return m_projection->zNear();
}
void AbstractCamera::setZFar(const float z)
{
    m_projection->setZFar(z);
    invalidate();
}
const float AbstractCamera::zFar() const
{
    return m_projection->zFar();
}
const glm::mat4 & AbstractCamera::projection() const
{
    return m_projection->projection();
}

void AbstractCamera::update()
{
    if(!m_invalid)
        return;

    recalculate();

    m_invalid = false;
}

void AbstractCamera::recalculate()
{
    m_viewProjection = projection() * m_view;//TODO different 'transform' interpretations (-> scene graph)

    m_viewFrustum->update();
}

void AbstractCamera::invalidate()
{
    if(m_invalid)
        return;
    m_invalid = true;
    // TODO filter for "significant" changes / provisionary
    m_viewChanged();
}

void AbstractCamera::invalidateChildren()
{
    Group::invalidateChildren();
}

void AbstractCamera::setView(const glm::mat4 & view)
{
    m_view = glm::mat4(view);
    invalidate();
}

const glm::mat4 & AbstractCamera::view() const
{
    return m_view;
}

const ViewFrustum * AbstractCamera::viewFrustum()
{
    update();
    return m_viewFrustum;
}

glm::vec3 AbstractCamera::eye() const
{
    //Get MonoCamera position (from: http://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix )

    glm::mat4 modelViewT = glm::transpose(m_view);

    // Get plane normals
    glm::vec3 n1(modelViewT[0]);
    glm::vec3 n2(modelViewT[1]);
    glm::vec3 n3(modelViewT[2]);

    // Get plane distances
    float d1(modelViewT[0].w);
    float d2(modelViewT[1].w);
    float d3(modelViewT[2].w);

    // Get the intersection of these 3 planes
    // (using math from RealTime Collision Detection by Christer Ericson)
    glm::vec3 n2n3 = glm::cross(n2, n3);
    float denom = glm::dot(n1, n2n3);

    glm::vec3 eye = (n2n3 * d1) + glm::cross(n1, (d3*n2) - (d2*n3));
    eye /= -denom;

    return eye;
}
glm::vec3 AbstractCamera::center() const
{
    glm::vec3 v_lookat = glm::row(m_view, 2).xyz();
    glm::vec3 v_eye = eye();

    return v_eye - v_lookat;

}
glm::vec3 AbstractCamera::up() const
{
    return glm::row(m_view, 1).xyz();
}

const signalzeug::Signal<> & AbstractCamera::viewChangedSignal()
{
    return m_viewChanged;
}
