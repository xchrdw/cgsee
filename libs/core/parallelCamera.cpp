#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "parallelCamera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");

ParallelCamera::ParallelCamera(const QString & name)
    : AbstractStereoCamera(name)
{
}


ParallelCamera::~ParallelCamera(void)
{
}

void ParallelCamera::activateRightCamera(const Program & program 
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 viewDirection = m_virtualCameraPosition - m_center;
    glm::normalize(viewDirection);

    glm::vec3 cameraPosition = m_virtualCameraPosition+=( m_cameraSeparationVector * m_cameraSeparation * (-1.f));

    setView(glm::lookAt(
        cameraPosition , cameraPosition += (viewDirection * 100.f), m_up));
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

void ParallelCamera::activateLeftCamera(const Program & program
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 viewDirection = m_virtualCameraPosition - m_center;
    glm::normalize(viewDirection);

    glm::vec3 cameraPosition = m_virtualCameraPosition+=( m_cameraSeparationVector * m_cameraSeparation);

    setView(glm::lookAt(
        cameraPosition , cameraPosition += (viewDirection * 100.f), m_up));
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

void ParallelCamera::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    return draw(program);
}

void ParallelCamera::draw(
    const Program & program
,   FrameBufferObject * target)
{
    if(m_invalidated)
        update();

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x, m_viewport.y);
    glError();

    program.setUniform(VIEWPORT_UNIFORM, m_viewport);

    setFromMatrix(m_view);

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateLeftCamera(program,target);
    activateRightCamera(program,target);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   

    if(target)
        target->release();
}