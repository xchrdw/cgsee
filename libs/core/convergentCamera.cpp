#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "convergentCamera.h"
#include "camera.h"
#include "program.h"
#include "gpuquery.h"
#include <core/screenquad.h>
#include <core/fileassociatedshader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/random.hpp>
#include <QDebug>


const QString ConvergentCamera::m_implementationName("ConvergentCamera");

const QString ConvergentCamera::implementationName() const
{
    return m_implementationName;
}

bool ConvergentCamera::isRegistered = CameraImplementation::registerImplementation(
    m_implementationName,
    createInstace<ConvergentCamera>);

ConvergentCamera::ConvergentCamera(Camera & abstraction)
    : AbstractStereoCamera(abstraction),
    m_focusDistance(5.0f)
{
}
 
ConvergentCamera::~ConvergentCamera(void)
{
}

void ConvergentCamera::initialize(const Program & program)
{
}

void ConvergentCamera::setCameraSeparation(float cameraSeparation)
{
    m_cameraSeparation = cameraSeparation;
}

void ConvergentCamera::setFocusDistance(float focusDistance)
{
    m_focusDistance = focusDistance;
}

void ConvergentCamera::activateRightCamera(const Program & program)
{
    glClear( GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    viewDirection = glm::normalize(viewDirection) * m_focusDistance;
    glm::vec3 focusCenter = m_virtualCameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, focusCenter, m_up));
    //m_abstraction.setTransform(m_projection * m_view); // done in update()
    //glm::mat4 transform = m_projection * m_view;

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
    //Group::draw(program, transform);
    abstractionRenderScene(program);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}

void ConvergentCamera::activateLeftCamera(const Program & program)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * -1.f);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    viewDirection = glm::normalize(viewDirection) * m_focusDistance;
    glm::vec3 focusCenter = m_virtualCameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, focusCenter, m_up));
    //m_abstraction.setTransform(m_projection * m_view); // done in update()
    //glm::mat4 transform = m_projection * m_view;

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);
    //Group::draw(program, transform);    
    abstractionRenderScene(program);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}

void ConvergentCamera::drawScene(
    const Program & program
,   const glm::mat4 & transform)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glViewport(0, 0, m_viewport.x , m_viewport.y);
    //glError();

    setFromMatrix();

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateLeftCamera(program);
    activateRightCamera(program);

    m_abstraction.setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
}
