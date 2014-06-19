
#include <core/rendering/basicrenderstage.h>

#include <glm/mat4x4.hpp>

#include <core/framebufferobject.h>
#include <core/program.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/camera.h>

BasicRenderStage::BasicRenderStage(PipelinePainter & painter)
    : SceneRenderStage(painter)
{
    //m_program->attach(myFancyShader);
}

BasicRenderStage::~BasicRenderStage(void)
{
}

void BasicRenderStage::reloadShaders()
{
    //TODO
}

void BasicRenderStage::resize(const int width, const int height)
{
    //TODO
}
