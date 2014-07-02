
#include <core/camera/monocamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include <core/program.h>
#include <core/gpuquery.h>
#include <core/viewfrustum.h>

MonoCameraNew::MonoCameraNew(const QString & name, Projection * projection)
:   AbstractCamera(name)
,   m_viewFrustum(new ViewFrustum(this))
,   m_projection(projection)
,   m_view(glm::mat4())
{

}
MonoCameraNew::~MonoCameraNew()
{
    delete m_viewFrustum;
}

bool MonoCameraNew::isStereo()
{
    return false;
}

void MonoCameraNew::setView(const glm::mat4 & view)
{
    m_view = glm::mat4(view);
    invalidate();
}

const glm::mat4 & MonoCameraNew::view() const
{
    return m_view;
}

ViewFrustum * MonoCameraNew::viewFrustum()
{
    update();
    return m_viewFrustum;
}
glm::vec3 MonoCameraNew::eye() const
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
glm::vec3 MonoCameraNew::center() const
{
    glm::vec3 v_lookat = glm::row(m_view, 2).xyz();
    glm::vec3 v_eye = eye();

    return v_eye - v_lookat;

}
glm::vec3 MonoCameraNew::up() const
{
    return glm::row(m_view, 1).xyz();
}

void MonoCameraNew::setUniformsIn(const Program & program)
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

void MonoCameraNew::update()
{
    if(!m_invalid)
        return;
    AbstractCamera::update();

    setTransform(projection() * m_view);

    m_viewFrustum->update();
}
