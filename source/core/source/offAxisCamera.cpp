
#include <core/offaxiscamera.h>

const QString OffAxisCamera::m_implementationName("OffAxisCamera");

const QString OffAxisCamera::implementationName() const
{
    return m_implementationName;
}

// call registerImplementation to make this implementation available

//bool OffAxisCamera::isRegistered = CameraImplementation::registerImplementation(
//    m_implementationName,
//    createInstace<OffAxisCamera>);

OffAxisCamera::OffAxisCamera(Camera & abstraction) 
    : AbstractStereoCamera(abstraction)
{
}


OffAxisCamera::~OffAxisCamera(void)
{
}
