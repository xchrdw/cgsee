
#include <core/camera/stereocamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include <core/program.h>
#include <core/gpuquery.h>
#include <core/viewfrustum.h>

StereoCamera::StereoCamera(const QString & name)
: AbstractCamera(name)
, m_leftCamera(MonoCamera(QString(name).append("_left")))
, m_rightCamera(MonoCamera(QString(name).append("_right")))
{
    
}

StereoCamera::~StereoCamera()
{

}

const float StereoCamera::aspect() const
{
    // TODO
}

void StereoCamera::update()
{
    //TODO
}

void StereoCamera::setUniforms(const Program & program)
{
    //TODO
}

const glm::ivec2 & StereoCamera::viewport() const
{
    //TODO
}

void StereoCamera::setViewport(const glm::ivec2 & size)
{
    setViewport(size.x, size.y);
}

void StereoCamera::setViewport(const int width, const int height)
{
    //TODO
}

const glm::mat4 & StereoCamera::projection()
{
    //TODO
}

const glm::mat4 & StereoCamera::view() const
{
    //TODO
}

void StereoCamera::setView(const glm::mat4 & view)
{
    //TODO
}

const float StereoCamera::fovy() const
{
    //TODO
}

void StereoCamera::setFovy(const float fovy)
{
    //TODO
}

const float StereoCamera::zNear() const
{
    //TODO
}

void StereoCamera::setZNear(const float z)
{
    //TODO
}

const float StereoCamera::zFar() const
{
    //TODO
}

void StereoCamera::setZFar(const float z)
{
    //TODO
}

ViewFrustum * StereoCamera::viewFrustum()
{
    //TODO
}

glm::vec3 StereoCamera::getEye() const
{
    //TODO
}

glm::vec3 StereoCamera::getCenter() const
{
    //TODO

}

glm::vec3 StereoCamera::getUp() const
{
    //TODO
}
