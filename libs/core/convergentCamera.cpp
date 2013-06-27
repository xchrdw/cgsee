#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "convergentCamera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"

//#include <core/fileassociatedshader.h>

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");

ConvergentCamera::ConvergentCamera(const QString & name)
    : AbstractStereoCamera(name),
    m_focusDistance(5.0f)
{
}

ConvergentCamera::~ConvergentCamera(void)
{
}

void ConvergentCamera::activateRightCamera(const Program & program 
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    viewDirection = glm::normalize(viewDirection) * m_focusDistance;
    glm::vec3 focusCenter = m_virtualCameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, focusCenter, m_up));
    setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;

    printf("view Direction: %f %f %f \n", viewDirection.x,viewDirection.y,viewDirection.z);
    printf("virt cam pos: %f %f %f \n", m_virtualCameraPosition.x,m_virtualCameraPosition.y,m_virtualCameraPosition.z);
    printf("focusCenter: %f %f %f \n\n", focusCenter.x,focusCenter.y,focusCenter.z);

    update();
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
    Group::draw(program, transform);
}

void ConvergentCamera::activateLeftCamera(const Program & program
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * -1.f);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    viewDirection = glm::normalize(viewDirection) * m_focusDistance;
    glm::vec3 focusCenter = m_virtualCameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, focusCenter, m_up));
    setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;

    update();

    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);
    Group::draw(program, transform);    
}

void ConvergentCamera::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    return draw(program);
}

void ConvergentCamera::draw(
    const Program & program
,   FrameBufferObject * target)
{
    if(m_invalidated)
        update();

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float sqrt2 = 1.41421356;

   // glViewport(0, 0, m_viewport.x / sqrt2 , m_viewport.y);
   // glError();

    setFromMatrix(m_view);

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    glViewport(0, 0, m_viewport.x / sqrt2, m_viewport.y);
    glError();
    update();
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    activateLeftCamera(program,target);

    glViewport(m_viewport.x / sqrt2, 0, m_viewport.x / sqrt2, m_viewport.y);
    glError();
    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    update();
    activateRightCamera(program,target);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
   

    if(target)
        target->release();
}