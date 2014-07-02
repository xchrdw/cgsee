
#include <core/camera/abstractcamera.h>#

#include <core/camera/projection.h>

const QString AbstractCamera::VIEWPORT_UNIFORM("viewport");
const QString AbstractCamera::VIEW_UNIFORM("view");
const QString AbstractCamera::PROJECTION_UNIFORM("projection");
const QString AbstractCamera::TRANSFORM_UNIFORM("transform");
const QString AbstractCamera::TRANSFORMINVERSE_UNIFORM("transformInverse");
const QString AbstractCamera::ZNEAR_UNIFORM("znear");
const QString AbstractCamera::ZFAR_UNIFORM("zfar");
const QString AbstractCamera::CAMERAPOSITION_UNIFORM("cameraposition");

AbstractCamera::AbstractCamera(const QString & name)
:   Group(name)
,   m_invalid(true)
{
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
}

AbstractCamera::~AbstractCamera()
{

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
    //actions?
    m_invalid = false;
}

void AbstractCamera::invalidate()
{
    if(m_invalid)
        return;
    //actions?
    m_invalid = true;
}

void AbstractCamera::invalidateChildren()
{
    Group::invalidateChildren();
}
