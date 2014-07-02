
#include <core/camera/stereocamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include <core/program.h>
#include <core/gpuquery.h>
#include <core/viewfrustum.h>

#include <core/camera/monocamera.h>

StereoCamera::StereoCamera(const QString & name, Projection * projection)
: AbstractCamera(name)
, m_projection(projection)
{
    m_leftCamera = new MonoCameraNew(QString(name).append("_left"), projection);
    m_rightCamera = new MonoCameraNew(QString(name).append("_right"), projection);
}

StereoCamera::~StereoCamera()
{
    delete m_leftCamera;
    delete m_rightCamera;
}

bool StereoCamera::isStereo()
{
    return true;
}

void StereoCamera::setView(const glm::mat4 & view)
{
    //TODO implement stereo transformation!
    m_leftCamera->setView(view);
    m_rightCamera->setView(view);
}

const MonoCameraNew * StereoCamera::leftCamera() const
{
    return m_leftCamera;
}
const MonoCameraNew * StereoCamera::rightCamera() const
{
    return m_rightCamera;
}

void StereoCamera::update()
{
    //TODO m_*Camera->setView(/*fancy transformed matrix*/);
}
