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
    
    FrameBufferObject *fboLeftCamera = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    FrameBufferObject *fboRightCamera = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();

    setFromMatrix();

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateLeftCamera(program,fboLeftCamera);
    activateRightCamera(program,fboRightCamera);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
   

    if(target)
        target->release();
}