#include "monocamera.h"

const QString MonoCamera::m_implementationName("MonoCamera");

const QString MonoCamera::implementationName() const
{
    return m_implementationName;
}

bool MonoCamera::isRegistered = CameraImplementation::registerImplementation(
    m_implementationName,
    createInstace<MonoCamera>);

MonoCamera::MonoCamera(Camera & abstraction) 
    : AbstractStereoCamera(abstraction)
{
}

MonoCamera::~MonoCamera(void)
{
}

void MonoCamera::drawScene( const Program & program, const glm::mat4 & transform)
{
    abstractionRenderScene(program);
}