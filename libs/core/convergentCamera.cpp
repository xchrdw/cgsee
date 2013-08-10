#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "convergentCamera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/random.hpp>
#include <QDebug>


//#include <core/fileassociatedshader.h>

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");

ConvergentCamera::ConvergentCamera(const QString & name)
    : AbstractStereoCamera(name),
    m_focusDistance(5.0f),
    m_aFramebuffer(-1)
{
    m_aTexture[0]=-1;
    m_aTexture[1]=-1;
}

ConvergentCamera::~ConvergentCamera(void)
{
}

void ConvergentCamera::initialize(const Program & program)

{
    if (m_aFramebuffer == -1) {

        glGenTextures(2, m_aTexture);

        glBindTexture(GL_TEXTURE_2D, m_aTexture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, m_aTexture[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_viewport.x, m_viewport.y, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glError();

        glGenFramebuffers(1, &m_aFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_aFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_aTexture[0], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_aTexture[1], 0);

        glError();

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        glError();

        if(GL_FRAMEBUFFER_COMPLETE != status)
            qDebug("Stereo Rendering Frame Buffer Object incomplete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glError();

    }

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
/*
void ConvergentCamera::draw(
    const Program & program
,   FrameBufferObject * target)
{
    if(m_invalidated)
        update();
   

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();

    setFromMatrix();

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateLeftCamera(program,target);
    activateRightCamera(program,target);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
   

    if(target)
        target->release();
}*/

void ConvergentCamera::draw(
    const Program & program
,   FrameBufferObject * target)
{
    initialize(program);
     if(m_invalidated)
        update();
    FrameBufferObject *left = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    FrameBufferObject *right = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1, true);
    if(left){
        left->bind();
    }
    left->bindTexture2D(program, "left",0);
  // if(target)
   //    glBindFramebuffer(GL_FRAMEBUFFER, target->m_fbo);
       //target->bind();
  /*   glActiveTexture(GL_TEXTURE0 + 0);

    glBindTexture(GL_TEXTURE_2D, m_aTexture[0]);
    glError();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);//m_aFramebuffer);
    glError();

   GLenum writebuffers[] = { GL_COLOR_ATTACHMENT0 + 0,0};

    glDrawBuffers(2, writebuffers);
    glError();*/

    setFromMatrix();
    m_cameraSeparationVector = glm::normalize(glm::cross(m_center-m_virtualCameraPosition , m_up));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    activateLeftCamera(program,nullptr);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));

 /*   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //if(target)
    //    target->bind();
    // copy written buffer to screen/output framebuffer

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_aFramebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + 0);
    glError();

    glBlitFramebuffer(0, 0, m_viewport.x, m_viewport.y, 0, 0, m_viewport.x, m_viewport.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);*/







    /*

    setFromMatrix();
    m_cameraSeparationVector = glm::normalize(glm::cross(m_center-m_virtualCameraPosition , m_up));
    
    FrameBufferObject *fboLeftCamera = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    FrameBufferObject *fboRightCamera = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
   
    //leftCamera
    t_samplerByName samplerLeft;
    samplerLeft["leftTexture"] = fboLeftCamera;
    

    GLenum info = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glError();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    bindSampler(samplerLeft, program);
    if(fboLeftCamera)
    {    fboLeftCamera->bind();}
       
    activateLeftCamera(program,fboLeftCamera);
    releaseSampler(samplerLeft);

    //rightCamera
     if(fboRightCamera)
        fboRightCamera->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();

    activateRightCamera(program,fboRightCamera);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
   */
    if(left){
        left->release();
    }
   // if(target)
    //  target->release();
}