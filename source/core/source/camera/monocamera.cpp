
#include <core/camera/monocamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include <core/program.h>
#include <core/gpuquery.h>
#include <core/viewfrustum.h>

MonoCamera::MonoCamera(const QString & name)
    : AbstractCamera(name)
    , m_viewFrustum(new ViewFrustum(this))
    , m_fovy(0.f)
    , m_zNear(0.f)
    , m_zFar(0.f)
{
    
}

MonoCamera::~MonoCamera()
{
    delete m_viewFrustum;
}

const float MonoCamera::aspect() const
{
    return static_cast<float>(m_viewport.x) / static_cast<float>(m_viewport.y);
}

void MonoCamera::update()
{
    m_projection = glm::perspective(m_fovy, aspect(), m_zNear, m_zFar);

    setTransform(m_projection * m_view);

    m_invalidated = false;

    m_viewFrustum->update();
}

void MonoCamera::setUniforms(const Program & program)
{
    if (m_invalidated)
        update();
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
    program.setUniform(TRANSFORM_UNIFORM, m_transform);
    program.setUniform(TRANSFORMINVERSE_UNIFORM, m_transformInverse);
    program.setUniform(CAMERAPOSITION_UNIFORM, getEye());
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    //    for(int row = 0; row <4; row++)
    //    {
    //        qDebug() << "TRANSFORM[" << row << "] " << m_transform[row].x << " " << m_transform[row].y << " " << m_transform[row].z << " " << m_transform[row].w;
    //    }
}

const glm::ivec2 & MonoCamera::viewport() const
{
    return m_viewport;
}

void MonoCamera::setViewport(const glm::ivec2 & size)
{
    setViewport(size.x, size.y);
}

void MonoCamera::setViewport(const int width, const int height)
{
    m_viewport = glm::ivec2(width, height);
    invalidate();
}

const glm::mat4 & MonoCamera::projection()
{
    if (m_invalidated)
        update();

    return m_projection;
}

const glm::mat4 & MonoCamera::view() const
{
    return m_view;
}

void MonoCamera::setView(const glm::mat4 & view)
{
    m_view = view;
    invalidate();
}

const float MonoCamera::fovy() const
{
    return m_fovy;
}

void MonoCamera::setFovy(const float fovy)
{
    if (fovy == m_fovy)
        return;

    m_fovy = fovy;
    invalidate();
}

const float MonoCamera::zNear() const
{
    return m_zNear;
}

void MonoCamera::setZNear(const float z)
{
    if (z == m_zNear)
        return;

    m_zNear = z;
    invalidate();
}

const float MonoCamera::zFar() const
{
    return m_zFar;
}

void MonoCamera::setZFar(const float z)
{
    if (z == m_zNear)
        return;

    m_zFar = z;
    invalidate();
}

ViewFrustum * MonoCamera::viewFrustum()
{
    if (m_invalidated)
        update();
    return m_viewFrustum;
}

glm::vec3 MonoCamera::getEye() const
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

glm::vec3 MonoCamera::getCenter() const
{
    glm::vec3 lookat = glm::row(m_view, 2).xyz();
    glm::vec3 eye = getEye();

    return eye - lookat;

}

glm::vec3 MonoCamera::getUp() const
{
    return glm::row(m_view, 1).xyz();
}
