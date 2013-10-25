#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include "camera.h"

#include "cameraimplementation.h"

#include "rendering/rasterizer.h"
#include "rendering/pathtracer.h"

#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"
#include "core/viewfrustum.h"

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");
static const QString TRANSFORM_UNIFORM("transform");
static const QString TRANSFORMINVERSE_UNIFORM("transformInverse");
static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");
static const QString CAMERAPOSITION_UNIFORM ("cameraposition");

Camera::Camera(const QString & name)
:   Group(name)
,   m_viewFrustum(new ViewFrustum(this))
,   m_fovy(0.f)
,   m_zNear(0.f)
,   m_zFar (0.f)
,   m_invalidated(true)
// add a new camera of each implemented type to our list
,   m_implementations(CameraImplementation::newImplementations(*this))
,   m_activeCamera(nullptr)
,   m_rasterizer(new Rasterizer(*this))
,   m_pathTracer(new PathTracer(*this))
,   m_activeRenderTechnique(nullptr)
{
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
    selectImplementation("MonoCamera");
    selectRenderingByName("Rasterization");
}

Camera::~Camera()
{
    qDeleteAll(m_implementations);
    delete m_rasterizer;
    delete m_pathTracer;
}

void Camera::selectImplementation(QString name)
{
    m_activeCamera = nullptr;

    for (CameraImplementation* impl: m_implementations) {
        if (impl->implementationName() == name) {
            m_activeCamera = impl;
            // update selected camera
            m_activeCamera->onInvalidatedView();
            m_activeCamera->onInvalidatedViewport(m_viewport.x, m_viewport.y);
            m_activeCamera->onInvalidatedChildren();
            return;
        }
    }
    if (m_activeCamera == nullptr) {
        qDebug()<<"Selected camera"<<name<<"which is not implemented.";
        exit(2);
    }
}

bool Camera::selectRenderingByName(QString name)
{
    if (name == "Rasterization") {
        m_activeRenderTechnique = m_rasterizer;
        return true;
    }
    if (name == "PathTracing") {
        m_activeRenderTechnique = m_pathTracer;
        return true;
    }
    return false;
}

QString Camera::selectedImplementation()
{
    return m_activeCamera->implementationName();
}

CameraImplementation * Camera::activeImplementation() const
{
    return m_activeCamera;
}

int Camera::preferredRefreshTimeMSec() const
{
    return m_activeRenderTechnique->preferredRefreshTimeMSec();
}

void Camera::drawScene( const Program & program, const glm::mat4 & transform )
{
    if (m_activeCamera == nullptr)
        return;

    if(m_invalidated)
        update();   // refresh matrices

    // do not call glClear here, as it is not needed in all implementations

    glViewport(0, 0, m_viewport.x, m_viewport.y);
    glError();

    setUniforms(program);

    m_activeCamera->drawScene(program, transform);
}

void Camera::renderScene(const Program & program)
{
    m_activeRenderTechnique->renderScene(program, m_transform);
}

void Camera::invalidate()
{
    if(m_invalidated)
        return;

    m_invalidated = true;

    if (m_activeCamera)
        m_activeCamera->onInvalidatedView();
}

void Camera::invalidateChildren()
{
    Group::invalidateChildren();

    if (m_activeCamera)
        m_activeCamera->onInvalidatedChildren();
}

const float Camera::aspect() const
{
    return static_cast<float>(m_viewport.x) / static_cast<float>(m_viewport.y);
}

void Camera::update()
{
    m_projection = glm::perspective(m_fovy, aspect(), m_zNear, m_zFar);

    setTransform(m_projection * m_view);

    m_invalidated = false;

    m_viewFrustum->update();
}

void Camera::setUniforms(const Program & program) const
{
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
    program.setUniform(TRANSFORM_UNIFORM, m_transform);
    program.setUniform(TRANSFORMINVERSE_UNIFORM, m_transformInverse);
    program.setUniform(CAMERAPOSITION_UNIFORM, getEye());
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
}

const glm::ivec2 & Camera::viewport() const
{
    return m_viewport;
}

void Camera::setViewport(const glm::ivec2 & size)
{
    setViewport(size.x, size.y);
}

void Camera::setViewport(
    const int width
,   const int height)
{
    m_viewport = glm::ivec2(width, height);
    invalidate();
    if (m_activeCamera)
        m_activeCamera->onInvalidatedViewport(width, height);
}

const glm::mat4 & Camera::projection()
{
    if(m_invalidated)
        update();

    return m_projection;
}

const glm::mat4 & Camera::view() const
{
    return m_view;
}

void Camera::setView(const glm::mat4 & view)
{
    m_view = view;
    invalidate();
}

const float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if(fovy == m_fovy)
        return;

    m_fovy = fovy;
    invalidate();
}

const float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float z)
{
    if(z == m_zNear)
        return;

    m_zNear = z;
    invalidate();
}

const float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float z)
{
    if(z == m_zNear)
        return;

    m_zFar = z;
    invalidate();
}

ViewFrustum *Camera::viewFrustum() const {
    return m_viewFrustum;
}

Camera * Camera::asCamera()
{
    return this;
}

glm::vec3 Camera::getEye() const
{
    //Get Camera position (from: http://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix )

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

glm::vec3 Camera::getCenter() const
{
    glm::vec3 lookat = glm::row(m_view, 2).xyz;
    glm::vec3 eye = getEye();

    return eye - lookat;

}

glm::vec3 Camera::getUp() const
{
    return glm::row(m_view, 1).xyz;
}