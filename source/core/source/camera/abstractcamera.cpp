
#include <core/camera/abstractcamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <core/camera/projection.h>
#include <core/viewfrustum.h>
#include <core/program.h>

const QString AbstractCamera::VIEWPORT_UNIFORM("viewport");
const QString AbstractCamera::VIEW_UNIFORM("view");
const QString AbstractCamera::PROJECTION_UNIFORM("projection");
const QString AbstractCamera::TRANSFORM_UNIFORM("transform");
const QString AbstractCamera::TRANSFORMINVERSE_UNIFORM("transformInverse");
const QString AbstractCamera::ZNEAR_UNIFORM("znear");
const QString AbstractCamera::ZFAR_UNIFORM("zfar");
const QString AbstractCamera::CAMERAPOSITION_UNIFORM("cameraposition");

AbstractCamera::AbstractCamera(const QString & name, Projection * projection)
:   Group(name)
,   m_invalid(true)
,   m_viewFrustum(new ViewFrustum(this))
,   m_projection(projection)
,   m_view(glm::mat4())
{
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
}

AbstractCamera::~AbstractCamera()
{
    delete m_viewFrustum;
}

void AbstractCamera::setProjection(Projection * projection)
{
    m_projection = projection;
}
const Projection * AbstractCamera::getProjection() const
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
    setTransform(projection() * m_view);//TODO different 'transform' interpretations (-> scene graph)

    m_viewFrustum->update();
}

void AbstractCamera::invalidate()
{
    if(m_invalid)
        return;
    m_invalid = true;
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

void AbstractCamera::setUniformsIn(const Program & program)
{
    program.setUniform(PROJECTION_UNIFORM, projection());
    program.setUniform(VIEWPORT_UNIFORM, m_projection->viewport());
    program.setUniform(ZNEAR_UNIFORM, m_projection->zNear());
    program.setUniform(ZFAR_UNIFORM, m_projection->zFar());

    program.setUniform(VIEW_UNIFORM, view());
    program.setUniform(TRANSFORM_UNIFORM, transform());
    program.setUniform(TRANSFORMINVERSE_UNIFORM, transformInverse());
    program.setUniform(CAMERAPOSITION_UNIFORM, eye());
}
