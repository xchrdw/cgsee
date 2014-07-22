
#include <core/camera/stereocamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <QDebug>

#include <core/program.h>
#include <core/gpuquery.h>
#include <core/viewfrustum.h>

#include <core/camera/monocamera.h>

StereoCamera::StereoCamera(const QString & name, Projection * projection)
: AbstractCamera(name, projection)
{
    m_leftCamera = new MonoCamera(QString(name).append("_left"), projection);
    m_rightCamera = new MonoCamera(QString(name).append("_right"), projection);
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

const MonoCamera * StereoCamera::leftCamera()
{
    update();
    return m_leftCamera;
}
const MonoCamera * StereoCamera::rightCamera()
{
    update();
    return m_rightCamera;
}

void StereoCamera::recalculate()
{
    AbstractCamera::recalculate();

    //TODO implement stereo transformation!

    m_leftCamera->setView(m_view);
    m_rightCamera->setView(m_view);
}
