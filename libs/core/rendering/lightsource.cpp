
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "lightsource.h"

#include "../program.h"
#include "../framebufferobject.h"
#include "../fileassociatedshader.h"
#include "../scenegraph/group.h"

LightSourcePass::LightSourcePass(Camera * camera, FileAssociatedShader * depth_util)
:   DefaultPass(camera)
,   m_scene(nullptr)
{
    m_lightcam = new Camera();
    m_lightcam->setViewport(camera->viewport());
    m_lightcam->setFovy(camera->fovy());
    m_lightcam->setZFar(camera->zFar());
    m_lightcam->setZNear(camera->zNear());

    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/lightsource.frag"));
    m_program->attach(depth_util);
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/lightsource.vert"));
}


LightSourcePass::~LightSourcePass(void)
{
}

void LightSourcePass::render()
{
    drawScene(m_lightcam, m_program, m_fbo);
}

void LightSourcePass::sceneChanged(Group * scene)
{
    if(m_scene)
        m_lightcam->remove(m_scene);
    m_lightcam->append(scene);

    AxisAlignedBoundingBox bb = scene->boundingBox();
    m_lightcam->setView(glm::lookAt(glm::vec3(3.5, 5.0, 5.5)+bb.center(),
                                    bb.center(), glm::vec3(0.0,1.0,0.0)));
    m_scene = scene;
}

void LightSourcePass::resize( const int width, const int height )
{
    m_lightcam->setViewport(width, height);
    m_lightcam->update();

    DefaultPass::resize(width, height);
}

glm::mat4 LightSourcePass::transform()
{
    return m_lightcam->transform();
}
