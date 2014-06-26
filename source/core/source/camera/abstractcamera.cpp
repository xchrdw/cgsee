
#include <core/camera/abstractcamera.h>

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
    ,   m_invalidated(true)
{
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
}

AbstractCamera::~AbstractCamera()
{

}

void AbstractCamera::invalidate()
{
    if(m_invalidated)
        return;

    m_invalidated = true;
}

void AbstractCamera::invalidateChildren()
{
    Group::invalidateChildren();
}