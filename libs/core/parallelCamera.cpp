#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "parallelCamera.h"
#include "camera.h"
#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"
#include <core/screenquad.h>
#include <core/fileassociatedshader.h>

const QString ParallelCamera::m_implementationName("ParallelCamera");

const QString ParallelCamera::implementationName() const
{
    return m_implementationName;
}

bool ParallelCamera::isRegistered = CameraImplementation::registerImplementation(
    m_implementationName,
    createInstace<ParallelCamera>);

ParallelCamera::ParallelCamera(Camera & abstraction)
    : AbstractStereoCamera(abstraction),
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

void ParallelCamera::setCameraSeparation(float cameraSeparation)
{
    m_cameraSeparation = cameraSeparation;
}

void ParallelCamera::activateOculusRift()
{
    m_oculus=true;
}

void ParallelCamera::deactivateOculusRift()
{
    m_oculus=false;
}

void ParallelCamera::activateRedCyanRightCamera(const Program & program)
{
    //glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, center, m_up));
    //setTransform(m_projection * m_view); // done in update
    //glm::mat4 transform = m_projection * m_view;

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);
    //Group::draw(program, transform);
    abstractionRenderScene(program);
}

void ParallelCamera::activateRedCyanLeftCamera(const Program & program)
{
    /*glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );*/

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * (-1.f));
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, center, m_up));
    /*setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;*/

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);
    //Group::draw(program, transform);    
    abstractionRenderScene(program);
}

void ParallelCamera::activateOculusRiftRightCamera(const Program & program, FrameBufferObject * target)
{
    //glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation);
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, center, m_up));
    /*setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;*/

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    //Group::draw(program, transform);
    abstractionRenderScene(program, target);
}

void ParallelCamera::activateOculusRiftLeftCamera(const Program & program, FrameBufferObject * target)
{
    //glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

    glm::vec3 cameraPosition = m_virtualCameraPosition+( m_cameraSeparationVector * m_cameraSeparation * (-1.f));
    glm::vec3 viewDirection = m_center - m_virtualCameraPosition;
    glm::normalize(viewDirection);

    glm::vec3 center = cameraPosition + viewDirection;

    m_abstraction.setView(glm::lookAt(
        cameraPosition, center, m_up));
    /*setTransform(m_projection * m_view);
    glm::mat4 transform = m_projection * m_view;*/

    m_abstraction.update();
    m_abstraction.setUniforms(program);

    //Group::draw(program, transform);   
    abstractionRenderScene(program, target);
}

void ParallelCamera::drawScene(
    const Program & program)
{
    if(m_oculus)
    {
        drawOculusRift(program);
    }
    else
    {
        drawRedCyan(program);
    }
}

void ParallelCamera::drawRedCyan(
    const Program & program)
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setFromMatrix();

    m_cameraSeparationVector = glm::cross(m_center-m_virtualCameraPosition , m_up);
    glm::normalize(m_cameraSeparationVector);

    activateRedCyanLeftCamera(program);
    activateRedCyanRightCamera(program);
    
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   
    m_abstraction.setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
}

void ParallelCamera::drawOculusRift(
    const Program & program)
{
    initialize(program);
    
    setFromMatrix();
    m_cameraSeparationVector = glm::normalize(glm::cross(m_center-m_virtualCameraPosition , m_up));

    const glm::ivec2 & viewport(m_abstraction.viewport()); // to shorten the lines in this function a bit ..

    m_left->resize(viewport.x, viewport.y);
    m_right->resize(viewport.x, viewport.y);

    ///
    //render left camera
    ///

    //m_left->bind();

    glViewport(0, 0, viewport.x, viewport.y);
    glError();
    
    activateOculusRiftLeftCamera(program, m_left);
    //m_left->release();


    ///
    //render right camera
    ///

    //m_right->bind();

    glViewport(0, 0, viewport.x , viewport.y);
    glError();
    
    activateOculusRiftRightCamera(program, m_right);
    //m_right->release();


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
    sideBySide->setUniform("viewport", viewport);

    bindSampler(sampler,*sideBySide);
    quad->draw(*sideBySide, nullptr);
    releaseSampler(sampler);

    //set camera position, center, up (view matrix) to original position
    m_abstraction.setView(glm::lookAt(m_virtualCameraPosition, m_center, m_up));
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