#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "convergentCamera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"
#include <core/screenquad.h>
#include <core/fileassociatedshader.h>

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
    left(nullptr),
    right(nullptr)
{
}

ConvergentCamera::~ConvergentCamera(void)
{
}

void ConvergentCamera::initialize(const Program & program)
{
    if(left == nullptr)
    {
        left = new FrameBufferObject(
            GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    }
    if( right == nullptr)
    {
        right = new FrameBufferObject(
            GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    }
}

void ConvergentCamera::activateRightCamera(const Program & program 
,   FrameBufferObject * target)
{
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    viewDirection = glm::normalize(viewDirection) * m_focusDistance;
    glm::vec3 focusCenter = m_virtualCameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, focusCenter, m_up));
    setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;

    //update();
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
  //  glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
    Group::draw(program, transform);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
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

    //update();

    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    //glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);
    Group::draw(program, transform);    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
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
    
    setFromMatrix();
    m_cameraSeparationVector = glm::normalize(glm::cross(m_center-m_virtualCameraPosition , m_up));

  //  glClear(GL_COLOR_BUFFER_BIT);

    left->resize(m_viewport.x, m_viewport.y);
    right->resize(m_viewport.x, m_viewport.y);

    ///
    //render left camera
    ///

    if(left){
        left->bind();
    }

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    
    activateLeftCamera(program,nullptr);
    left->release();


    ///
    //render right camera
    ///

    if(right){
        right->bind();
    }

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    
    activateRightCamera(program,nullptr);
    right->release();


    ///
    //combine both cameras – side-by-side-rendering / oculus rift
    ///

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    t_samplerByName sampler;
    sampler["leftCam"]=left;
    sampler["rightCam"]=right;

    ScreenQuad *quad = new ScreenQuad();
    Program *sideBySide = new Program();

    sideBySide->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));
    sideBySide->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/oculusRift.frag"));
    sideBySide->setUniform("viewport", m_viewport);
   
    target->bind();
    bindSampler(sampler,*sideBySide);
    quad->draw(*sideBySide, nullptr);
    releaseSampler(sampler);
    target->release();

    //set camera position, center, up (view matrix) to original position
    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));

 

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

   // if(target)
    //  target->release();
}

void ConvergentCamera::bindSampler(

    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void ConvergentCamera::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();

}