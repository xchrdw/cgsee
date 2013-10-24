#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "rasterizingcamera.h"

#include "gpuquery.h"

// define the virtual and the static member in each camera implementation to simulate "virtual static"
const QString RasterizingCamera::m_implementationName("RasterizationCamera");

const QString RasterizingCamera::implementationName() const
{
    return m_implementationName;
}

// Use registerImplementation in each implementation to register it in the abstract class.
// This allows the Camera to use all available implementations without having to put them
// in the Cameras class definition.
bool RasterizingCamera::isRegistered = CameraImplementation::registerImplementation(
    m_implementationName,
    createInstace<RasterizingCamera>);


RasterizingCamera::RasterizingCamera(Camera & abstraction)
:   CameraImplementation(abstraction)
{
}

RasterizingCamera::~RasterizingCamera()
{
}

void RasterizingCamera::draw( const Program & program, const glm::mat4 & transform )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glError();
}
