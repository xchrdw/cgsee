#include "convergentCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");

ConvergentCamera::ConvergentCamera(const QString & name)

    : AbstractStereoCamera(name)
{
}

ConvergentCamera::~ConvergentCamera(void)
{
}

void ConvergentCamera::activateRightCamera(const Program & program 
,   FrameBufferObject * target)//change const 
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 transform(1.0f);
    setView(glm::lookAt(
        glm::vec3( -2.0f, 0.0f,4.f) , glm::vec3( 1.f, -1.f, 0.f), glm::vec3( 1.f, 1.f, 1.f)));
    //transform *= //glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    //transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));
    
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);
    Group::draw(program, transform);
}
void ConvergentCamera::activateLeftCamera(const Program & program
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );
    glm::mat4 transform(1.f);
    transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));

    setView(glm::lookAt(
        glm::vec3( -4.0f, 0.0f,-2.f) , glm::vec3( 0.f, 0.f, 0.f), glm::vec3( 0.f, 1.f, 0.f)));
    
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
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

    glViewport(0, 0, m_viewport.x, m_viewport.y);
    glError();

    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    
   activateLeftCamera(program,target);
    activateRightCamera(program,target);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   

    if(target)
        target->release();
}