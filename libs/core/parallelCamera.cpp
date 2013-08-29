#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "parallelCamera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"
#include <core/screenquad.h>
#include <core/fileassociatedshader.h>

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");

ParallelCamera::ParallelCamera(const QString & name)
    : AbstractStereoCamera(name),
    m_left(nullptr),
    m_right(nullptr),
    m_oculus(true)
{
}


ParallelCamera::~ParallelCamera(void)
{
    delete m_left;
    delete m_right;
}

void ParallelCamera::initialize(const Program & program)
{
    if(m_left == nullptr)
    {
        m_left = new FrameBufferObject(
            GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    }
    if( m_right == nullptr)
    {
        m_right = new FrameBufferObject(
            GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    }
}

void ParallelCamera::activateOculusRift()
{
    m_oculus=true;
}

void ParallelCamera::deactivateOculusRift()
{
    m_oculus=false;
}

void ParallelCamera::activateRedCyanRightCamera(const Program & program 
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, center, m_up));
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

void ParallelCamera::activateRedCyanLeftCamera(const Program & program
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * (-1.f));
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, center, m_up));
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

void ParallelCamera::activateOculusRiftRightCamera(const Program & program 
,   FrameBufferObject * target)
{
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, center, m_up));
    setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;

    //update();

    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);

    Group::draw(program, transform);
}

void ParallelCamera::activateOculusRiftLeftCamera(const Program & program
,   FrameBufferObject * target)
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * (-1.f));
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    setView(glm::lookAt(
        cameraPosition, center, m_up));
    setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;

    //update();

    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);

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
    if(m_oculus)
    {
        drawOculusRift(program, target);
    }
    else
    {
        drawRedCyan(program, target);
    }
}

void ParallelCamera::drawRedCyan(
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

    setFromMatrix();

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateRedCyanLeftCamera(program,target);
    activateRedCyanRightCamera(program,target);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   
    setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));

    if(target)
        target->release();
}

void ParallelCamera::drawOculusRift(
    const Program & program
,   FrameBufferObject * target)
{
    initialize(program);
     if(m_invalidated)
        update();
    
    setFromMatrix();
    m_cameraSeparationVector = glm::normalize(glm::cross(m_center-m_virtualCameraPosition , m_up));

    m_left->resize(m_viewport.x, m_viewport.y);
    m_right->resize(m_viewport.x, m_viewport.y);

    ///
    //render left camera
    ///

    if(m_left){
        m_left->bind();
    }

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    
    activateOculusRiftLeftCamera(program,nullptr);
    m_left->release();


    ///
    //render right camera
    ///

    if(m_right){
        m_right->bind();
    }

    glViewport(0, 0, m_viewport.x , m_viewport.y);
    glError();
    
    activateOculusRiftRightCamera(program,nullptr);
    m_right->release();


    ///
    //combine both cameras – side-by-side-rendering / oculus rift
    ///

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    t_samplerByName sampler;
    sampler["leftCam"]=m_left;
    sampler["rightCam"]=m_right;

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
}

void ParallelCamera::bindSampler(
    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void ParallelCamera::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();
}